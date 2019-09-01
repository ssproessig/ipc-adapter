#pragma once

// everybody is going to use std::unique_ptr<>
#include <memory>

// Release builds reference logic_error in SANITY_CHECK
#include <stdexcept>

// readability: we want to use less space for forward declarations
#define FORWARD_DECLARE_IMPL(T, CONTAINER)                  \
CONTAINER T

#define FORWARD_DECLARE(T)                                  \
FORWARD_DECLARE_IMPL(T, class)

#define FORWARD_DECLARE_STRUCT(T)                           \
FORWARD_DECLARE_IMPL(T, struct)

#define FORWARD_DECLARE_NS(NS,T)                            \
namespace NS {                                              \
    FORWARD_DECLARE(T);                                     \
}

#define FORWARD_DECLARE_STRUCT_NS(NS,T)                     \
namespace NS {                                              \
    FORWARD_DECLARE_STRUCT(T);                              \
}

#define FORWARD_DECLARE_NS_NS(NS1,NS2,T)                    \
namespace NS1 {                                             \
  FORWARD_DECLARE_NS(NS2,T)                                 \
}

#define FORWARD_DECLARE_STRUCT_NS_NS(NS1,NS2,T)             \
namespace NS1 {                                             \
  FORWARD_DECLARE_STRUCT_NS(NS2,T)                          \
}

#define FORWARD_DECLARE_TEMPLATED(T)                        \
template <typename TN> FORWARD_DECLARE_IMPL(T, class)

#define FORWARD_DECLARE_TEMPLATED_2(T)                      \
template <typename KEY, typename VALUE>                     \
    FORWARD_DECLARE_IMPL(T, class)

#define FORWARD_DECLARE_TEMPLATED_NS(NS,T)                  \
namespace NS {                                              \
    FORWARD_DECLARE_TEMPLATED(T);                           \
}

#define FORWARD_DECLARE_ENUM(T, INTEGRAL_TYPE)              \
    enum class T: INTEGRAL_TYPE;

#define FORWARD_DECLARE_SHARED_PTR(NAME, WRAPPED_TYPE)      \
    FORWARD_DECLARE(WRAPPED_TYPE);                          \
    using NAME = std::shared_ptr<WRAPPED_TYPE>;


// readability: when we need to return a static string from a method (that returns QString const&)
// note: we define a separate scope to not collide w/ local variables
#define RETURN_STATIC_STRING(content)                   \
{                                                       \
    static QString const& s = QStringLiteral(content);  \
    return s;                                           \
}


// static and constant initialization - lazy-loading, w/o using C++ static initialization at startup
#define DECLARE_CONST(T,NAME,INITIALIZER) \
T const& NAME()                           \
{                                         \
    static T const aConstant INITIALIZER; \
    return aConstant;                     \
}

#define DECLARE_CONST_VARIADIC(T,NAME,...)  \
T const& NAME()                             \
{                                           \
    static T const aConstant __VA_ARGS__;   \
    return aConstant;                       \
}

#define DECLARE_SINGLETON(T,NAME,INITIALIZER) \
    T& NAME()                                 \
{                                             \
    static T aConstant INITIALIZER;           \
    return aConstant;                         \
}


// private data implementation; real internal data structure must be added in implementation
#define PIMPLED_DATA(d)     \
    private:                \
        struct Data;        \
        std::unique_ptr<Data> const d


// macro stringification helper
#define STRINGIFY(s) #s

// we want to have a readable version of the "interface" destructor
#define INTERFACE_DESTRUCTOR(iface) virtual ~iface() = default
#define INTERFACE_DESTRUCTOR_NOEXCEPT(iface) virtual ~iface() noexcept = default


#define INTERFACE_CONSTRUCTOR(iface) \
    iface() = default;               \
    iface(iface const&) = default

#define INTERFACE_NONCOPYABLE(iface) \
    iface() = default;               \
    iface(iface const&) = delete

// No copy constructor
#define NONCOPYABLE(clazz) \
    clazz(clazz const&) = delete; \
    clazz& operator=(clazz const&) = delete

// Static utility class. May not be instantiated
#define STATIC_CLASS(clazz) \
    clazz() = delete;       \
    ~clazz() = delete;      \
    NONCOPYABLE(clazz)

// a macro used for control flow checks where a condition MUST NOT happen in the control flow (execution can not continue if conditions fails)
#if !defined(SANITY_CHECK)
#  if defined(QT_NO_DEBUG) && !defined(QT_FORCE_ASSERTS)
#    define SANITY_CHECK(cond) ((!(cond)) ? throw std::logic_error(#cond) : qt_noop())
#  else
#    define SANITY_CHECK(cond) ((!(cond)) ? qt_assert(#cond,__FILE__,__LINE__) : qt_noop())
#  endif
#endif

// some macros we will use for later documentation generation -> no code is being created
#define REALIZE_REQUIREMENT(requirement_id) qt_noop()
