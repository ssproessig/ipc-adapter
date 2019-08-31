# Coding rules

## C++

### General rules
* The formatting rules for source files described in the [Naming Conventions](Naming_Conventions.md) are mandatory.
* implementation: all code committed to version control MUST be compile clean. Code written in anticipation of a future extension and which has dependencies that cannot be met yet MUST be encapsulated in `#if defined _FUTURE_` and annotated with a `@FIXME` annotation until it is compile clean.
* `#include` of project headers in a different directory MUST be specified with full path relative to project root. Path specifications containing parent directories (`../`) are NOT allowed.
* `#include`s MUST be listed in the following order
    * project includes
    * Qt includes
    * standard library includes
* Within each category, the `#include`s MUST be listed in alphabetical order, considering the entire path.
* implementation: the following keywords MUST be used to annotate work-in-progress:
    * use `@FIXME:` to note code that is known to be incomplete or broken
    * use `@TODO:` to note code that needs to be refactored
    * use `@REVIEW:` to note code that needs special attention by the tasks reviewer

### Scope of declaration (Forward Declaration)
* headers: you MUST use forward declarations whenever possible.
* forward declarations MUST be defined via the provided macros (`FORWARD_DECLARE*`) for readability

### Do not publish implementation details (PIMPL pattern)
* headers: public classes MUST NOT have private members besides a *pimpled data* member.
* headers: a class is public when its header is intended to be included by other modules, i.e. resides in the `api` directory. All other classes SHOULD place all private data in the *pimpled data* member and MUST justify exceptions in a comment.
* implementation: private code MUST be put into the anonymous namespace

### Prevent unintended copying of objects (Rule of Five)
* headers: you MUST explicitly signal that a class is copy-able by adding a *copy constructor* and an *assignment operator* (remember the *Rule of Three*!); in any other case a class MUST signal being non-copyable by using the `NONCOPYABLE(clazz)` macro as first line in the class definition
* headers: you MUST explicitly signal that a class is *static* (can not be instantiated nor copied, has only static members) by using the `STATIC_CLASS(clazz)` macro

### Provide abstract interfaces
* in order to make mocking in unit tests easy, classes SHOULD always define their public interface as abstract interface, then derive from that abstract interface (refer to coding standard in _Sutter, Alexandrescu: C++ Coding Standards, Boston 2005, pp. 62_). Classes SHOULD publish that abstract interface only, and never the concrete implementation.
* warning: when declaring an interface, it MUST be made sure that this interface never inherits from another interface directly  (to avoid the "dreaded diamond" and usage of virtual inheritance, see http://www.cs.technion.ac.il/users/yechiel/c++-faq/mi-diamond.html)
* if there MAY be more than one way to implement an algorithm in a component, it SHOULD be checked if it may be an option to extract the implementation as an extension instead of hard-coding it into the component

### Write modern C++11 code
* you MUST favor `using typename = definition` over the legacy `typedef`
* you MUST use scoped enumerations (`enum class`) instead of `enum`
* you SHOULD specify a base type for enumerations (`enum ... : basetype`)
* all fields of the `enum class` MUST be documented
* you SHALL use `auto` whenever possible, use `auto const&` where applicable
* you MUST be `const`-clean in your interfaces (document the intent of the API better)
* overridden methods MUST be identified with the `override` keyword.
* prefer enforcing explicit construction by providing the `explicit` keyword to constructors with a single parameter. If implicit conversion is desired, this MUST be documented with a `/* implicit */` comment.

### Lifetime of objects: usage of pointers and references
General rule: The lifetime for any object MUST be well defined. That means: who creates an object MUST also define when that object is destroyed.

Passing objects around makes is more difficult to control their lifetime. Therefore the following rules SHOULD be observed:

* The lifetime of any object created with `new` SHOULD be controlled by a smart pointer.
* Use a `std::unique_ptr<>` whenever possible. This is especially the case when a single owner of the created object can be identified. Passing a unique pointer transfers the ownership and lifetime control to the callee. The compiler enforces that move semantics are used for such a transfer. Therefor the caller's copy of the pointer will be invalid after the call.
* Use `std::shared_ptr<>` when an object needs to be referenced by several parties which can not be controlled from a single place.
Caution: Use of shared pointers is generally more complicated and more expensive than unique pointers. Special care must be taken to avoid reference loops: any structure created with shared pointers must be a directed acyclical graph. When it is necessary to add references which could be cyclic, `std::weak_ptr<>` MUST be used. Weak pointers do not participate in controlling the lifetime, but can be converted to a (strong) shared pointer at any time.
* Passing a `std::shared_ptr<>` allows the callee to participate in the control of the lifetime of the referenced object. This applies to both weak and strong pointers, because a weak pointer can be converted to a strong pointer at any time.
* Passing references `typename &` or raw pointers  `typename *` from objects controlled by smart pointers is allowed *if and only if* it is guaranteed that the callee will not access the passed object after is has been deleted. A common situation where this applies is when the lifetime of the callee is shorter than the lifetime of the passed object.
* Passing a raw pointer allows the callee to access the referenced object as long as the lifetime constraints described above are met. In particular the callee is allowed to store the pointer for its own use.
* Passing a reference `typename &` allows the callee to access the referenced object for the *duration of the call only*. The callee must not make any assumption on the lifetime of the object beyond the duration of the call. In particular the callee should not store or forward a pointer from the passed reference.

#### Checking for `nullptr`
With a few exceptions, pointers MUST always be checked for being valid. The exceptions are:
* pointers resulting from calls which throw an exception on failure need not to be checked. Example: creating an object with `new` either creates a valid pointer or throws an exception. Therefore the pointer does not need to be checked.
* a (strong) shared pointer cannot vanish beyond the control of the owner. Therefore it needs to be checked only when created from a weak pointer or obtained via `dynamic_pointer_cast<>`.

When passing an object, either caller or callee MUST ensure that the object is valid.
* When passing an object as reference, it is always the responsibility of the caller to ensure a valid object reference
* When passing an object as pointer of any kind, caller and callee MUST agree on who is responsible for checking the pointer. This MUST be documented in the API documentation. E.g. when a callee cannot accept a `nullptr`, its API documentation should mention a phrase like "must not be null". It is then the responsibility of the caller to ensure that the passed pointer is valid.
* Defensive programming is always recommended. Be prepared that other parts of the system violate rules. Therefore provide means of early detecting such violations. The macro `SANITY_CHECK` can be used for that purpose. E.g. use a statement like `SANITY_CHECK(pointer != nullptr)` for a pointer that should have been checked by the caller. This will produce a `Q_ASSERT` in debug builds, and throw an exception in release builds.
