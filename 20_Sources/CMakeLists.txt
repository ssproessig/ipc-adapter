cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

# get some absolute paths we can share amongst all projects
get_filename_component(IPC_ADAPTER_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# we use shared project settings for all our CMake projects
include(${IPC_ADAPTER_PATH}/CMake/ProjectSettings.cmake)

include_directories(IpcAdapter)

add_subdirectory(IpcAdapter/Application)

include(CPack)
