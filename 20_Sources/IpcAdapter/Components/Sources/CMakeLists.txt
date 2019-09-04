set(LIB_NAME IpcAdapterSources)

set(SRC
    src/UdpSource.cpp
    src/UdpSource.h
)
source_group("Source Components Implementation" ${TST})

set(DOC
    doc/Sources_MRS.md
)
source_group("Source Components Documentation" ${DOC})

set(TST
    tst/UdpSourceTest.cpp
    tst/UdpSourceTest.h
    tst/TestMain.cpp
)
source_group("Source Components UnitTests" ${TST})

set(LNK
    Qt5::Core
    Qt5::Network
)

add_library(${LIB_NAME} OBJECT ${DOC} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_executable(${LIB_NAME}Test ${DOC} ${TST})
target_link_libraries(${LIB_NAME}Test ${LNK} $<TARGET_OBJECTS:IpcAdapterSources> IpcAdapterCore Qt5::Test)
