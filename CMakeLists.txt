cmake_minimum_required(VERSION 2.8)

#SET(USD_LOCATION "$ENV{REZ_USDBASE_ROOT}")
set(CMAKE_FOLDER ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
set(STUDIOEXAMPLE_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(STUDIOSCHEMAS_INCLUDE_DIRS ${STUDIOEXAMPLE_SRC_ROOT}/StudioSchemas)
set(STUDIOMAYANODES_INCLUDE_DIRS ${STUDIOEXAMPLE_SRC_ROOT}/StudioMayaNodes)

include(${CMAKE_FOLDER}/ALConfig.cmake)
add_subdirectory(StudioSchemas)
add_subdirectory(StudioMayaNodes)
add_subdirectory(StudioTranslators)
