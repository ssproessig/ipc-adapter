set(APP_NAME IpcAdapterApplication)

set(SRC
    src/IpcAdapterApplication.cpp
)

source_group(Sources ${SRC})
add_executable(${APP_NAME} ${SRC}
    $<TARGET_OBJECTS:AmqpComponents>
    $<TARGET_OBJECTS:Converters>
    $<TARGET_OBJECTS:NetworkComponents>
)

set(LNK_LIBS
    IpcAdapterCore
    qamqp-lib
)

set(LNK ${LNK_LIBS}
    Qt5::Core
    Qt5::Network
    Qt5::Xml
)
target_link_libraries(${APP_NAME} ${LNK})
