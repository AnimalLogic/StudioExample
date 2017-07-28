cmake_minimum_required(VERSION 2.8)

# set_property(GLOBAL PROPERTY PXR_STUDIO_SCHEMAS_PACKAGE "StudioSchemas")
set(PXR_PACKAGE_STUDIO_SCHEMAS "StudioSchemas")
set(PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS _${PXR_PACKAGE_STUDIO_SCHEMAS})

add_subdirectory(Studio)

# The following configuration will spit out a StudioSchemasConfig.cmake 
# file whose goal is to provide locations to header files/libraries and such.
# currently it isn't needed since no outside package compiles nor links against it.
# I'll keep it here for an example.
get_target_property(PXR_PYTHON_LIBRARY_LOCATION ${PXR_PYTHON_PACKAGE_STUDIO_SCHEMAS} LIBRARY_LOCATION2)
get_target_property(PXR_LIBRARY_LOCATION ${PXR_PACKAGE_STUDIO_SCHEMAS} LIBRARY_LOCATION2)

#Configure and install this packages *config.cmake file
configure_file( StudioSchemasConfig.cmake.in
                ${PROJECT_BINARY_DIR}/StudioSchemasConfig.cmake @ONLY)

#Install the libraries CmakeConfig.
install(FILES ${PROJECT_BINARY_DIR}/StudioSchemasConfig.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  COMPONENT dev
)
