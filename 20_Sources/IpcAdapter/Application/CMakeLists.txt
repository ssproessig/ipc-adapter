set(APP_NAME IpcAdapterApplication)

set(SRC
    src/IpcAdapterApplication.cpp
)

source_group(Sources ${SRC})
add_executable(${APP_NAME} ${SRC})

set(LNK_LIBS
    $<TARGET_OBJECTS:IpcAdapterConverters>
    $<TARGET_OBJECTS:IpcAdapterSinks>
    $<TARGET_OBJECTS:IpcAdapterSources>
    IpcAdapterCore
    qamqp-lib
)

set(LNK ${LNK_LIBS}
    Qt5::Core
    Qt5::Network
    Qt5::Xml
)
target_link_libraries(${APP_NAME} ${LNK})
