# usd api
include(pxrConfig)
include_directories(${PXR_INCLUDE_DIRS})

# usd cmake config helpers
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/defaults
    ${CMAKE_CURRENT_LIST_DIR}/modules
    ${CMAKE_CURRENT_LIST_DIR}/macros)

# Verbose Makefiles by default.
set(CMAKE_VERBOSE_MAKEFILE OFF)

# CXXDefaults will set a variety of variables for the project.
# Consume them here.  This is an effort to keep the most common
# build files readable.
include(CXXDefaults)
add_definitions(${_PXR_CXX_DEFINITIONS})
set(CMAKE_CXX_FLAGS
    ${_PXR_CXX_FLAGS})

# needed by _install_python function
set(PYTHON_EXECUTABLE ${python_ROOT}/bin/python)

include(ProjectDefaults)
# to get pxr_shared_library function
include(Public)
################################################################################
# copy pxr_setup_python to add namespace package definition
function(al_setup_python)
    get_property(pxrPythonModules GLOBAL PROPERTY PXR_PYTHON_MODULES)

    # A new list where each python module is quoted
    set(converted "")
    foreach(module ${pxrPythonModules})
        list(APPEND converted "'${module}'")
    endforeach()

    # Join these with a ', '
    string(REPLACE ";" ", " pyModulesStr "${converted}")

    # Install a pxr __init__.py with an appropriate __all__
    _get_install_dir(lib/python/pxr installPrefix)
    install(CODE
        "file(WRITE \"${CMAKE_INSTALL_PREFIX}/${installPrefix}/__init__.py\" \"try:\n\t__import__('pkg_resources').declare_namespace(__name__)\nexcept:\n\tpass\n__all__ = [${pyModulesStr}]\n\")"
    )
endfunction()
################################################################################
macro(al_get_schema_sources pathToSource cppFiles pymoduleCppFiles publicHeaderFiles)
    #Determine the Python Wrap files
    FILE(GLOB ${pymoduleCppFiles} "${pathToSource}/wrap*.cpp")
    list(APPEND ${pymoduleCppFiles} "${pathToSource}/module.cpp")

    #Determine CPP files
    FILE(GLOB ${cppFiles} "${pathToSource}/*.cpp")
    list(REMOVE_ITEM ${cppFiles} ${pymoduleCppFiles})
    list(REMOVE_ITEM ${cppFiles} "${pathToSource}/module.cpp")

    #Determine Header files
    FILE(GLOB ${publicHeaderFiles} "${pathToSource}/*.h")
endmacro()
################################################################################
macro(al_get_sources pathToSource outCppFiles outHeaderFiles)
    #Determine CPP files
    FILE(GLOB ${outCppFiles} "${pathToSource}/*.cpp")
    #Determine Header files
    FILE(GLOB ${outHeaderFiles} "${pathToSource}/*.h")
endmacro()
################################################################################
        
function(al_build_and_install_translators PACKAGE_NAME)
    set(oneValueArgs 
        DIRECTORY_PATH
    )
    set(multiValueArgs
          CPP_FILES
          PUBLIC_HEADER_FILES
          INCLUDE_DIRS
          DEPENDANT_LIBRARIES
        )

    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # usd plugin
     add_library(${PACKAGE_NAME}
         SHARED
         ${arg_CPP_FILES}
         ${arg_PUBLIC_HEADER_FILES}
     )

     target_include_directories(
         ${PACKAGE_NAME}
         PUBLIC  ${arg_INCLUDE_DIRS}

     )

     set_target_properties(${PACKAGE_NAME}
         PROPERTIES COMPILE_DEFINITIONS
             "MFB_PACKAGE_NAME=${PACKAGE_NAME};MFB_ALT_PACKAGE_NAME=${PACKAGE_NAME}"
     )

     target_link_libraries(${PACKAGE_NAME} ${arg_DEPENDANT_LIBRARIES})

     install(TARGETS ${PACKAGE_NAME}
         LIBRARY
         DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
     )

     #Configure and install the plugInfo.json
     _plugInfo_subst(${PACKAGE_NAME} plugInfo.json)

     install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json
         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/usd/plugins/${PACKAGE_NAME}/resources
     )

    #install top level plugInfo.json that includes the configured plugInfo.json
     install(CODE
         "file(WRITE \"${CMAKE_INSTALL_PREFIX}/share/usd/plugins/plugInfo.json\" \"{\n    \\\"Includes\\\": [ \\\"*/resources/\\\" ]\n}\")"
     )

     # install public headers
     install(FILES ${arg_PUBLIC_HEADER_FILES}
             DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${DIRECTORY_PATH}
     )

endfunction(al_build_and_install_translators)
################################################################################
