# Naming Conventions

## Naming and Formatting Rules for XML Schema Definitions

### XSD-001
All identifiers SHALL be in American English.

### XSD-002 
Lower case SHALL be used for all identifiers.

### XSD-003 
Hyphens SHALL be used for structuring names.

### XSD-004
Each root element SHALL have a  mandatory `version` attribute.

### XSD-005
Version numbers SHALL be a simple integer `xsd:unsignedInt`.

### XSD-006 
A qualified namespaces MUST be used.

### XSD-007 
The namespace identifiers SHALL use the `urn:` syntax.

### XSD-008 
The namespace identifiers SHALL begin with a `ipca-` prefix and end with a `-v`_`n`_ version suffix where _`n`_ is the version as defined in XSD-005.

### XSD-009 
When nesting elements, a parent element SHALL have at least one attribute.

Rationale: don't introduce parent elements that only collect children, as this only adds parsing overhead.

### Example
```xml
<ipca version="1" xmlns="urn:ipca-pipeline-v1">
    <sources>
        <source id="UdpSource">
            <param name="port" value="9876" />
            <param name="host" value="127.0.0.1" />
        </source>
    </source>
</ipca>
```


## Naming and Formatting Rules for directories
### DIR-001
Directory names SHALL use `PascalCase`.

### DIR-002 
Directories MAY have an optional numeric prefix for providing a sort order.

### DIR-003
If specified, the numeric prefix SHALL have the same length for all directories of the same level.

### DIR-004 
As an exception, "`_`" may be used as prefix to force a single file to the beginning of the directory listing. This "`_`" counts as uppercase character.


## Code formatting
### CFMT-001
AStyle SHALL be used for code formatting, see http://astyle.sourceforge.net/astyle.html.

### CFMT-002
The following AStyle options SHALL be used:

| **command-line switch**   | **meaning**            |
|---------------------------|------------------|
| `--style=allman`  | [Allmann style brackets](http://astyle.sourceforge.net/astyle.html#_style=allman) |
| `--indent=spaces=4`  | NEVER use tabs, but [replace them with 4 spaces](http://astyle.sourceforge.net/astyle.html#_indent=spaces)     |
| `--min-conditional-indent=0`  | [reformat multi-line conditionals](http://astyle.sourceforge.net/astyle.html#_min-conditional-indent) w/o indentation             |
| `--max-instatement-indent=60` | [set the maximum # spaces to indent a continuation line](http://astyle.sourceforge.net/astyle.html#_max-instatement-indent)        |
|`--align-pointer=type`|[attach the pointer or reference operator to the type](http://astyle.sourceforge.net/astyle.html#_align-pointer), not the variable, as recommended by [Mr. C++](http://www.stroustrup.com/bs_faq2.html#whitespace) himself.|
|`--max-code-length=200`|[no line must ever exceed 200 characters](http://astyle.sourceforge.net/astyle.html#_max-code-length)|
|`--indent-switches`|[indent switches](http://astyle.sourceforge.net/astyle.html#_indent-switches)|
|`--indent-namespaces`|[indent namespaces](http://astyle.sourceforge.net/astyle.html#_indent-namespaces)|
|`--indent-col1-comments`|[Align comments before an instruction to the instruction](http://astyle.sourceforge.net/astyle.html#_indent-col1-comments)|
|`--break-blocks`|[Pad empty lines around header blocks ](http://astyle.sourceforge.net/astyle.html#_break-blocks)|
|`--pad-oper`|[insert space padding around operators](http://astyle.sourceforge.net/astyle.html#_pad-oper). Any end of line comments will remain in the original column, if possible.|
|`--pad-header`|[Insert space padding between a header](http://astyle.sourceforge.net/astyle.html#_pad-header)  (e.g. 'if', 'for', 'while'...) and the following paren. Any end of line comments will remain in the original column, if possible.|
|`--unpad-paren`|[remove extra space padding](http://astyle.sourceforge.net/astyle.html#_unpad-paren) around parenthesis on the inside and outside|
|`--add-brackets`|[enforces brackets](http://astyle.sourceforge.net/astyle.html#_add-brackets) to unbracketed one line conditional statements (e.g. 'if', 'for', 'while'...).|
|`--mode=c`|[use C/C++ mode](http://astyle.sourceforge.net/astyle.html#_mode)|
|`--suffix=none`|[do NOT create a backup](http://astyle.sourceforge.net/astyle.html#_suffix) of the file (otherwise there will be a [original].orig file)|

All options MAY be placed in a options file named `${HOME}/.astylerc` as follows

```
    style=allman
    indent=spaces=4
    min-conditional-indent=0
    max-instatement-indent=60
    align-pointer=type
    max-code-length=200
    indent-switches
    indent-namespaces
    indent-col1-comments
    break-blocks
    pad-oper
    pad-header
    unpad-paren
    add-brackets
    mode=c
    suffix=none
```