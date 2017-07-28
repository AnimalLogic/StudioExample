cmake_minimum_required(VERSION 2.8)
find_package(Maya REQUIRED)

set(STUDIO_MAYA_NODES_LIBRARY_NAME "StudioMayaNodes")
set(STUDIO_MAYA_NODES_PLUGIN_NAME "StudioMayaNodesPlugin")
add_subdirectory(Studio/nodes)
add_subdirectory(Studio/plugin)