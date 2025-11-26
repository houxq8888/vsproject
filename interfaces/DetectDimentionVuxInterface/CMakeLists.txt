set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)


HG_CMAKE_ADD_MODELU(DetectDimentionVuxInterface inc src)


HG_CMAKE_MODULE_DEPEND_PUB( 
    HXQDetectDimentionVuxModule
    HGImageAlgorithmService
    HGCommon
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
)

HG_CMAKE_ADD_SHARED(HXQDetectDimentionVuxInterface inc src)

HG_CMAKE_MODULE_DEPEND(
    HXQDetectDimentionVuxModule
    HGImageAlgorithmService
    HGCommon
    ${ZXING_LIBRARY}
    ${LOG4CPLUS_LIBRARIES}
)
set_target_properties(HXQDetectDimentionVuxInterface PROPERTIES 
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    SOVERSION ${LIBRARY_VERSION_MAJOR}
)