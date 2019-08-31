set(LIB_NAME IpcAdapterCore)

file(GLOB API api/*.h)
file(GLOB HDR src/*.h)

set(SRC
    ${HDR}
    src/ComponentRegistry.cpp
    src/SimplePipelineFrame.cpp
    src/Runtime.cpp
)

set(TST
    tst/ComponentRegistryTest.h
    tst/ComponentRegistryTest.cpp
    tst/TestMain.cpp
)


source_group("Core API" ${API})
source_group("Core Sources" ${SRC})

add_library(${LIB_NAME} STATIC ${API} ${SRC})
target_link_libraries(${LIB_NAME} Qt5::Core)

add_executable(${LIB_NAME}Test ${DOC} ${SRC} ${TST})
target_link_libraries(${LIB_NAME}Test ${LNK} Qt5::Test)
