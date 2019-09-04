file(GLOB CPP_FILES qamqp/*.cpp)
file(GLOB H_FILES qamqp/*.h)


add_library(qamqp-lib STATIC ${CPP_FILES} ${H_FILES})
target_link_libraries(qamqp-lib Qt5::Core Qt5::Network)
target_include_directories(qamqp-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
