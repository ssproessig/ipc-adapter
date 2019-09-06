# determine the package version
set(CPACK_PACKAGE_VERSION_DEVELOPMENT "0.0.0")

# if no PACKAGE version is set via CLI parameters, set it to development
if (NOT CPACK_PACKAGE_VERSION)
    message(STATUS "! no CPACK_PACKAGE_VERSION parameter seen. Defaulting to development version ${CPACK_PACKAGE_VERSION_DEVELOPMENT}")
    set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_DEVELOPMENT})
endif()

# extract version information to separate numbers
string(REPLACE "." ";" VERSION_LIST ${CPACK_PACKAGE_VERSION})
list(GET VERSION_LIST 0 CPACK_PACKAGE_VERSION_MAJOR)
list(GET VERSION_LIST 1 CPACK_PACKAGE_VERSION_MINOR)
list(GET VERSION_LIST 2 CPACK_PACKAGE_VERSION_PATCH)

# append the git hash to it
set(CPACK_PACKAGE_VERSION_SIMPLE ${CPACK_PACKAGE_VERSION})
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}-${VCS_COMMIT_ID})
message(STATUS "  this is version ${CPACK_PACKAGE_VERSION}.")

# general package options
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "IpcAdapter is a configurable pipeline IPC processor")
set(CPACK_PACKAGE_NAME "IpcAdapter")
set(CPACK_PACKAGE_VENDOR "Sören Sprößig")
set(CPACK_PACKAGE_CONTACT ssproessig@gmail.com)

# NOTE: 'include(CPack)'  MUST happen after all CPACK_ variables are set in the including sub directories (e.g. for menu links)

# a helper function to add VCS information to the FILE_NAME passed
function(_add_cpack_package_version_info_to_file FILE_NAME)
    set_property(
        SOURCE ${FILE_NAME}
        APPEND
        PROPERTY COMPILE_DEFINITIONS
        VCS_BRANCH="${VCS_BRANCH}" VCS_COMMIT_ID="${VCS_COMMIT_ID}" CPACK_PACKAGE_VERSION="${CPACK_PACKAGE_VERSION}" CPACK_PACKAGE_VERSION_SIMPLE="${CPACK_PACKAGE_VERSION_SIMPLE}"
    )
endfunction()
