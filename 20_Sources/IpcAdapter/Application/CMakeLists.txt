set(APP_NAME IpcAdapterApplication)

set(SRC
    src/IpcAdapterApplication.cpp
)

source_group(Sources ${SRC})
add_executable(${APP_NAME} ${SRC})
target_link_libraries(${APP_NAME} IpcAdapterCore Qt5::Core)
