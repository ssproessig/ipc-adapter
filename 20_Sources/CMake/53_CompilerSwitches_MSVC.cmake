# disable manifest generation
# read here: http://stackoverflow.com/a/8359871
set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
set(CMAKE_MODULE_LINKER_FLAGS /MANIFEST:NO)
set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)


function(_disable_warning_for_unsecure_function_calls FILE_NAME)
    message(STATUS "      MSVC: adding '_CRT_SECURE_NO_WARNINGS' for '${FILE_NAME}'")
    set_property(
        SOURCE ${FILE_NAME}
        APPEND
        PROPERTY COMPILE_DEFINITIONS
        "_CRT_SECURE_NO_WARNINGS=1"
    )
endfunction()
