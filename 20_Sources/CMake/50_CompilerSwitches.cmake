# settings to apply to all compilers
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# define some empty function stubs that some compilers may overwrite
function(_disable_warning_for_unsecure_function_calls FILE_NAME)
endfunction()


# compiler specific settings
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    include(${CMAKE_CURRENT_LIST_DIR}/51_CompilerSwitches_GCC.cmake)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    include(${CMAKE_CURRENT_LIST_DIR}/52_CompilerSwitches_Clang.cmake)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    include(${CMAKE_CURRENT_LIST_DIR}/53_CompilerSwitches_MSVC.cmake)
endif()

# finally we support building "releases with symbol"
include(${CMAKE_CURRENT_LIST_DIR}/59_StripDebugSymbolsInRelease.cmake)
