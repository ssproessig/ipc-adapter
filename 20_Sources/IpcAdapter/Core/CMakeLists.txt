set(LIB_NAME IpcAdapterCore)

file(GLOB API api/*.h)

set(SRC
    src/Runtime.cpp
)

source_group("Core API" ${API})
source_group("Core Sources" ${SRC})

add_library(${LIB_NAME} STATIC ${API} ${SRC})
target_link_libraries(${LIB_NAME} Qt5::Core)
