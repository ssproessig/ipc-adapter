set(LIB_NAME IpcAdapterCore)

file(GLOB API api/*.h)
file(GLOB HDR src/*.h)

set(SRC
    ${HDR}
    src/ComponentRegistry.cpp
    src/ConfigurationHandler.cpp
    src/GlobalComponentRegistry.cpp
    src/SimplePipelineFrame.cpp
    src/Pipeline.h
    src/Pipeline.cpp
    src/Runtime.cpp
    src/RuntimeConfiguration.cpp
)

set(TST
    tst/ComponentRegistryTest.h
    tst/ComponentRegistryTest.cpp
    tst/PipelineTest.h
    tst/PipelineTest.cpp
    tst/RuntimeTest.h
    tst/RuntimeTest.cpp
    tst/RuntimeTest.qrc
    tst/TestMain.cpp
)


source_group("Core API" ${API})
source_group("Core Sources" ${SRC})

set(LNK Qt5::Core Qt5::Xml)

add_library(${LIB_NAME} OBJECT ${API} ${SRC})
target_link_libraries(${LIB_NAME} ${LNK})

add_qtest(NAME ${LIB_NAME}Test SOURCES ${TST} $<TARGET_OBJECTS:${LIB_NAME}> LIBS ${LNK})
