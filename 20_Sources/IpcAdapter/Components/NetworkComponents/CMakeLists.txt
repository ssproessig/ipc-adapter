set(LIB_NAME NetworkComponents)

set(SRC
    src/UdpSink.cpp
    src/UdpSink.h
    src/UdpSource.cpp
    src/UdpSource.h
)
source_group("Implementation" ${TST})

set(DOC
    doc/NetworkComponents_MRS.md
)
source_group("Documentation" ${DOC})

set(TST
    tst/UdpSinkTest.cpp
    tst/UdpSinkTest.h
    tst/UdpSourceTest.cpp
    tst/UdpSourceTest.h
    tst/TestMain.cpp
)
source_group("UnitTests" ${TST})

set(LNK
    Qt5::Core
    Qt5::Network
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_qtest(NAME ${LIB_NAME}Test SOURCES ${TST} $<TARGET_OBJECTS:${LIB_NAME}> LIBS ${LNK} IpcAdapterCore)
