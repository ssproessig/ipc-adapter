set(LIB_NAME Converters)

set(SRC
    src/ReverseBytesConverter.cpp
    src/ReverseBytesConverter.h
    src/XmlToMetaDataConverter.cpp
    src/XmlToMetaDataConverter.h
)
source_group("Implementation" ${TST})

set(DOC
    doc/Converters_MRS.md
)
source_group("Documentation" ${DOC})

set(TST
    tst/ReverseBytesConverterTest.cpp
    tst/ReverseBytesConverterTest.h
    tst/XmlToMetaDataConverterTest.cpp
    tst/XmlToMetaDataConverterTest.h
    tst/TestMain.cpp
)
source_group("UnitTests" ${TST})

set(LNK
    Qt5::Core Qt5::Xml
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_qtest(NAME ${LIB_NAME}Test SOURCES ${TST} $<TARGET_OBJECTS:${LIB_NAME}> LIBS ${LNK} IpcAdapterCore)
