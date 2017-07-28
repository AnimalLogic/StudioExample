#path to where the public headers and other resources will be installed
if(DEFINED MAYA_LOCATION)
    find_package(Maya REQUIRED)
endif()

##
al_get_sources(${CMAKE_CURRENT_SOURCE_DIR} CPP_FILES PUBLIC_HEADER_FILES)
list(APPEND ALL_CPP_FILES ${CPP_FILES})
list(APPEND ALL_HEADER_FILES ${PUBLIC_HEADER_FILES} )
list(APPEND DEPENDANT_LIBRARIES StudioMayaNodes ${ALUSDMAYA_LIBRARY} ${PXR_PACKAGE_STUDIO_SCHEMAS} gf plug tf)
##

al_get_sources(${CMAKE_CURRENT_SOURCE_DIR} CPP_FILES PUBLIC_HEADER_FILES)

##################################################################################################################################################################################################################

list(APPEND ALL_INCLUDE_DIRS ${MAYA_INCLUDE_DIRS} ${AL_USDMaya_INCLUDE_DIRS} ${STUDIOSCHEMAS_INCLUDE_DIRS} ${STUDIOMAYANODES_INCLUDE_DIRS})
                 
al_build_and_install_translators(${PXR_PACKAGE_STUDIO_TRANSLATORS}
          DIRECTORY_PATH "StudioTranslators"
          CPP_FILES ${CPP_FILES}
          PUBLIC_HEADER_FILES ${PUBLIC_HEADER_FILES}
          INCLUDE_DIRS ${ALL_INCLUDE_DIRS}
          PXR_INCLUDE_DIRS ${PXR_INCLUDE_DIRS}
          DEPENDANT_LIBRARIES ${DEPENDANT_LIBRARIES}
)