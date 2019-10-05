# IpcAdapter Converter Components

## R-IPCA-CONVERTER-001: Provide ReverseBytes converter
The system SHALL provide a converter called `ReverseBytes` that reverses the bytes passed to it.

Example: processing `ABC123` SHALL convert to `321CBA`.


## R-IPCA-XML2METACNV-001: Provide XmlToMetaDataConverter converter
The system SHALL provide a converter called `XmlToMetaDataConverter` that parses the passed raw data as XML and forwards the elements and attributes as meta-data.

Example:  receiving
```xml
<root>
  <element id="1" other="attribute">
    <another info="here"/>
  </element>
</root>
```
SHALL create the following meta-data

```
root.element.id=1
root.element.other=attribute
root.element.another.info=here
```
in the frame forwarded. The frame

## R-IPCA-XML2METACNV-002: Configure XmlToMetaDataConverter converter
The `XmlToMetaDataConverter` SHALL support the following parameters:

- `filter-on-broken-xml` defaults to `true`:  if broken XML is received, the converter SHALL stop the pipeline and not forward the data
- `forward-raw-data` defaults to `false`:  if XML parsing succeeded, also forward the raw data with the meta-data
