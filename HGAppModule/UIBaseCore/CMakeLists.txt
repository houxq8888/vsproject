set(LIBRARY_VERSION_MAJOR 1)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

file(GLOB MOC_HEADERS "inc/*.h")

if (PLATFORM STREQUAL "x64")
    set(QT_LIBRARIES
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
    )
    qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
elseif (PLATFORM STREQUAL "win32")
    set(QT_LIBRARIES
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
    )
    qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
endif()

HG_CMAKE_ADD_MODULE(UIBaseCore inc src ${MOC_SRCS})

if(BUILD_SHARED_LIBS)
    if(WIN32)
        message(STATUS "Windows平台：只包含头文件")
    else()
        HG_CMAKE_MODULE_DEPEND_PUB(
            HGServiceInterfaces
            ${QT_LIBRARIES}
        )
        message(STATUS "Building HGUIBaseCore as shared library")
        HG_CMAKE_ADD_SHARED(HGUIBaseCore inc src ${MOC_SRCS})
        HG_CMAKE_MODULE_DEPEND(
            HGServiceInterfaces
            ${QT_LIBRARIES}
        )
        set_target_properties(HGUIBaseCore PROPERTIES 
            VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
            SOVERSION ${LIBRARY_VERSION_MAJOR}
        )
    endif()
endif()

if(BUILD_STATIC_LIBS)
    message(STATUS "Building HGUIBaseCoreStatic as static library")
    HG_CMAKE_ADD_STATIC(HGUIBaseCoreStatic inc src ${MOC_SRCS})
    
    if(WIN32)
        message(STATUS "Windows平台：只包含头文件")
        target_link_libraries(HGUIBaseCoreStatic PUBLIC 
            HGServiceInterfaces
            ${QT_LIBRARIES}
        )
    else()
        message(STATUS "Linux平台：包含整个库")
        HG_CMAKE_MODULE_DEPEND_PUB(
            HGServiceInterfaces
            ${QT_LIBRARIES}
        )
    endif()
endif()
