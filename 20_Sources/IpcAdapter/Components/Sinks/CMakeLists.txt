set(LIB_NAME IpcAdapterSinks)

set(SRC
    src/UdpSink.cpp
    src/UdpSink.h
    src/AmqpExchangeSink.cpp
    src/AmqpExchangeSink.h
)
source_group("Sinks Implementation" ${TST})

set(DOC
    doc/Sinks_MRS.md
)
source_group("Sinks Documentation" ${DOC})

set(TST
    tst/UdpSinkTest.cpp
    tst/UdpSinkTest.h
    tst/AmqpExchangeSinkTest.cpp
    tst/AmqpExchangeSinkTest.h
    tst/SinkTestMain.cpp
)
source_group("Sinks UnitTests" ${TST})

set(LNK
    qamqp-lib
    Qt5::Core
    Qt5::Network
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_executable(${LIB_NAME}Test ${DOC} ${TST})
target_link_libraries(${LIB_NAME}Test ${LNK} $<TARGET_OBJECTS:IpcAdapterSinks> IpcAdapterCore Qt5::Test)
