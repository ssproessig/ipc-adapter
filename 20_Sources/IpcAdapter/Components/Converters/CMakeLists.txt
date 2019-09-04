set(LIB_NAME IpcAdapterConverters)

set(SRC
    src/ReverseBytesConverter.cpp
    src/ReverseBytesConverter.h
)
source_group("Converters Implementation" ${TST})

set(DOC
    doc/Converters_MRS.md
)
source_group("Converters Documentation" ${DOC})

set(TST
    tst/ReverseBytesConverterTest.cpp
    tst/ReverseBytesConverterTest.h
    tst/TestMain.cpp
)
source_group("Converters UnitTests" ${TST})

set(LNK
    Qt5::Core
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_executable(${LIB_NAME}Test ${DOC} ${TST})
target_link_libraries(${LIB_NAME}Test ${LNK} $<TARGET_OBJECTS:IpcAdapterConverters> IpcAdapterCore Qt5::Test)
