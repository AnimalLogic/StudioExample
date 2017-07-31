cmake_minimum_required(VERSION 2.8)

list(APPEND studio_plugin_source
  plugin.cpp 
)

#need to link against AL_USDMaya
add_library( ${STUDIO_MAYA_NODES_PLUGIN_NAME}
  SHARED
  ${studio_plugin_source}
)

target_include_directories(${STUDIO_MAYA_NODES_PLUGIN_NAME}
    PUBLIC
    ../
)

target_link_libraries(${STUDIO_MAYA_NODES_PLUGIN_NAME}
    ${STUDIO_MAYA_NODES_LIBRARY_NAME}
    ${MAYA_Foundation_LIBRARY}
)

install( TARGETS ${STUDIO_MAYA_NODES_PLUGIN_NAME}
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_PREFIX}/plugin
)
