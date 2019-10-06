set(LIB_NAME AmqpComponents)

set(SRC
    src/AmqpConfigurable.cpp
    src/AmqpConfigurable.h
    src/AmqpConfiguration.cpp
    src/AmqpConfiguration.h
    src/AmqpExchangeSink.cpp
    src/AmqpExchangeSink.h
)
source_group("Implementation" ${TST})

set(DOC
    doc/AmqpComponents_MRS.md
)
source_group("Documentation" ${DOC})

set(TST
    tst/AmqpConfigurableTest.cpp
    tst/AmqpConfigurableTest.h
    tst/AmqpExchangeSinkTest.cpp
    tst/AmqpExchangeSinkTest.h
    tst/QAmqpSpyObjects.h
    tst/TestMain.cpp
)
source_group("UnitTests" ${TST})

set(LNK
    qamqp-lib
    Qt5::Core
    Qt5::Network
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_qtest(NAME ${LIB_NAME}Test SOURCES ${TST} $<TARGET_OBJECTS:${LIB_NAME}> LIBS ${LNK} IpcAdapterCore)
