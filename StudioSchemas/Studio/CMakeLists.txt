set(DIRECTORY_PATH StudioSchemas)

include(pxrConfig)

set(PYTHON_MODULE schemas)

# Copy plugInfo.json file so we can use as template
configure_file (
  "plugInfo.json.in"
  "plugInfo.json"
  @ONLY
)

# Edit __init__.py to contain module name (i.e. PXR_PACKAGE)
configure_file (
  "__init__.py.in"
  "${CMAKE_CURRENT_BINARY_DIR}/__init__.py"
)

get_property(PYTHON_PACKAGE GLOBAL PROPERTY GLOBAL_PYTHON_PACKAGE)

# Make sure your USD_SCHEMA_PATH is set correctly else the schema.usda will not be configured properly
# The USD_SCHEMA_PATH needs to point to USD's schema.usda file, see below how you may need to manually install it because of a reported issue.
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/schema.usda.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/schema.usda"
)

# It's worth to note that usdGenSchema and schema.usda comes from the USD repository and isn't currently exported for external PxrPlugins, like this package, to use it.
# I've had to manually install these 2 files after build USD like so:
#    echo "</Workaround for https://github.com/PixarAnimationStudios/USD/issues/158: Manually installing the following:"
#    cp pxr/usd/lib/usd/schema.usda $BUILD_DIR/share/usd/plugins/usd/resources && echo "pxr/usd/lib/usd/schema.usda -> $BUILD_DIR/share/usd/plugins/usd/resources"
#    cp -R pxr/usd/lib/usd/codegenTemplates $BUILD_DIR/bin/codegenTemplates && echo "pxr/usd/lib/usd/codegenTemplates-> $BUILD_DIR/bin/codegenTemplates"
#    echo "Workaround/>"
execute_process(COMMAND usdGenSchema schema.usda WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)

list(APPEND DEPENDANT_LIBRARIES tf sdf usd vt )
list(APPEND RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/generatedSchema.usda)

al_get_schema_sources(${CMAKE_CURRENT_SOURCE_DIR} CPP_FILES PYMODULE_CPPFILES PUBLIC_HEADER_FILES)

##################################################################################################################################################################################################################

include_directories(${PXR_INCLUDE_DIRS})

# usd plugin
add_library(${PXR_PACKAGE_STUDIO_SCHEMAS}
    SHARED
    ${CPP_FILES}
    ${PUBLIC_HEADER_FILES}
)

set_target_properties(${PXR_PACKAGE_STUDIO_SCHEMAS}
    PROPERTIES COMPILE_DEFINITIONS 
        "MFB_PACKAGE_NAME=${PXR_PACKAGE_STUDIO_SCHEMAS};MFB_ALT_PACKAGE_NAME=${PXR_PACKAGE_STUDIO_SCHEMAS};MFB_PACKAGE_MODULE=${PYTHON_MODULE}"
)

get_filename_component(
    PRIVATE_INC_DIR
    "${CMAKE_BINARY_DIR}/include"
    ABSOLUTE
)


target_include_directories(${PXR_PACKAGE_STUDIO_SCHEMAS}
    PRIVATE ${PRIVATE_INC_DIR}
)

target_link_libraries(${PXR_PACKAGE_STUDIO_SCHEMAS} ${DEPENDANT_LIBRARIES})

install(TARGETS ${PXR_PACKAGE_STUDIO_SCHEMAS}
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

set_target_properties(${PXR_PACKAGE_STUDIO_SCHEMAS} PROPERTIES LIBRARY_LOCATION2 ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${PXR_PACKAGE_STUDIO_SCHEMAS}${CMAKE_SHARED_LIBRARY_SUFFIX})

set_property(GLOBAL PROPERTY GLOBAL_PYTHON_PACKAGE ${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS})

# python bindings
add_library(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS}
    SHARED
    ${PYMODULE_CPPFILES}
)
set_target_properties(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS}
    PROPERTIES COMPILE_DEFINITIONS 

        "MFB_PACKAGE_NAME=${PXR_PACKAGE_STUDIO_SCHEMAS};MFB_ALT_PACKAGE_NAME=${PXR_PACKAGE_STUDIO_SCHEMAS};MFB_PACKAGE_MODULE=${PYTHON_MODULE}"
)

target_include_directories(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS}
    PRIVATE ${PRIVATE_INC_DIR}
)
set_target_properties(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS} 
    PROPERTIES 
        PREFIX ""
        FOLDER "${PXR_PREFIX}/_python"
)

target_link_libraries(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS} ${PXR_PACKAGE_STUDIO_SCHEMAS} ${DEPENDANT_LIBRARIES})

install(TARGETS ${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS}
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/${DIRECTORY_PATH}
)

set_target_properties(${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS} PROPERTIES LIBRARY_LOCATION2 ${CMAKE_INSTALL_PREFIX}/lib/python/${DIRECTORY_PATH}/${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS}${CMAKE_SHARED_LIBRARY_SUFFIX})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/${DIRECTORY_PATH}
)

string(REPLACE "/" ";" folderHierarchy "${DIRECTORY_PATH}")

list(LENGTH folderHierarchy listCount)
math(EXPR listMaxIndex "${listCount}-1")

#Install an __init__ file for all folders which will make up the python module path, except for the terminal folder which uses the checked in __init__ file.
if(${listCount} STRGREATER 1)
    set(currentPath "")
    foreach(i RANGE 0 ${listMaxIndex})
      if(${i} STREQUAL ${listMaxIndex})
        break()#we are at the last folder, we will use the checked in __init__.py for this folder.
      endif()
      set(currentFolder "")
      list(GET folderHierarchy ${i} currentFolder)
      list(APPEND currentPath ${currentFolder})

      string(REPLACE ";" "/" currentPath "${currentPath}")
      install(CODE
          "file(WRITE \"${CMAKE_INSTALL_PREFIX}/lib/python/${currentPath}/__init__.py\" \"try:\n\t__import__('pkg_resources').declare_namespace(__name__)\nexcept:\n\tpass\n\")"
      )
    endforeach(i)
endif()

_plugInfo_subst(${PXR_PACKAGE_STUDIO_SCHEMAS} plugInfo.json)

install(FILES ${RESOURCES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/usd/plugins/${PXR_PACKAGE_STUDIO_SCHEMAS}/resources
)

install(CODE
    "file(WRITE \"${CMAKE_INSTALL_PREFIX}/share/usd/plugins/plugInfo.json\" \"{\n    \\\"Includes\\\": [ \\\"*/resources/\\\" ]\n}\")"
)

# install public headers
install(FILES ${PUBLIC_HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${DIRECTORY_PATH}
)
#install the resources for the USD plugin
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/usd/plugins/${PXR_PACKAGE_STUDIO_SCHEMAS}/resources
)
