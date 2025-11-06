set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(module_fbow inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
#   log
#   perception_detection_segparse
#   avm
#   cv
)
HG_CMAKE_ADD_STATIC(fbow inc src)

HG_CMAKE_MODULE_DEPEND(
    # log
    # perception_detection_segparse
    # avm
    # cv
)
set_target_properties(fbow PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)
