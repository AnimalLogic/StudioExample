#add_definitions(${usdBase_DEFINITIONS}) need this for internal build

list(APPEND studio_nodes_public_headers
  UsdDrivenMeshExample.h
)

list(APPEND studio_plugin_public_headers
  PluginRegister.h
  TypeIDs.h
)

list(APPEND studio_plugin_headers
  ${studio_nodes_public_headers}
  ${studio_plugin_public_headers}
)

list(APPEND studio_plugin_source
  UsdDrivenMeshExample.cpp
)

add_library(${STUDIO_MAYA_NODES_LIBRARY_NAME}
  SHARED
  ${studio_plugin_headers}
  ${studio_plugin_source}
)

target_include_directories(
    ${STUDIO_MAYA_NODES_LIBRARY_NAME}
    PUBLIC
    ${studio_plugin_public_headers}
    ${STUDIOSCHEMAS_INCLUDE_DIRS}
    ${MAYA_INCLUDE_DIRS}
)

target_link_libraries(${STUDIO_MAYA_NODES_LIBRARY_NAME}
    AL_USDMaya
    StudioSchemas
    gf 
    kind
    sdf 
    tf
    usd
    vt
    ${MAYA_Foundation_LIBRARY}
    ${MAYA_OpenMayaAnim_LIBRARY}
    ${MAYA_OpenMayaFX_LIBRARY}
    ${MAYA_OpenMayaUI_LIBRARY}
    ${MAYA_OpenMaya_LIBRARY}
    ${MAYA_OpenMayaRender_LIBRARY}
)

install(  TARGETS ${STUDIO_MAYA_NODES_LIBRARY_NAME}
          LIBRARY
          DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

# install public headers
install( FILES ${studio_nodes_public_headers}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Studio/nodes
)

install( FILES ${studio_plugin_public_headers}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Studio/plugin
)
