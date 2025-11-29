project(HGTestPlugin VERSION 1.1.1)

file(GLOB MOC_HEADERS "inc/*.h")

# Define Qt libraries based on platform and compilation environment
if (PLATFORM STREQUAL "x64")
    # Linux native compilation - Qt5
    set(QT_LIBRARIES
        Qt5::Gui
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::PrintSupport
    )
    qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
elseif (PLATFORM STREQUAL "win32")
    if(WIN32)
        # Windows native compilation - Qt6
        set(QT_LIBRARIES
            Qt6::Gui
            Qt6::Core
            Qt6::Widgets
            Qt6::Charts
            Qt6::PrintSupport
        )
        # Qt6 uses automatic MOC, but also need explicit MOC processing for static libraries
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        # 确保所有包含Q_OBJECT的头文件都被MOC处理
        set_property(SOURCE ${MOC_HEADERS} PROPERTY SKIP_AUTOMOC FALSE)
        # 显式调用qt6_wrap_cpp确保MOC文件正确生成
        qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
    else()
        # Linux cross-compilation for Windows - Qt5
        set(QT_LIBRARIES
            Qt5::Gui
            Qt5::Core
            Qt5::Widgets
            Qt5::Charts
            Qt5::PrintSupport
        )
        qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
    endif()
endif()

HG_CMAKE_ADD_MODELU(TestPlugin inc src 
    ${MOC_SRCS})

target_compile_definitions(TestPlugin PUBLIC
     -DENV_CFG_FILE=\"\"
     -DENV_APP_NAME=\"HGTestPlugin\")

HG_CMAKE_MODULE_DEPEND_PUB(
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::UiPlugin
    Qt5::Designer
)

HG_CMAKE_ADD_SHARED(HGTestPlugin inc src ${MOC_SRCS})
HG_CMAKE_MODULE_DEPEND(
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::UiPlugin
    Qt5::Designer
)
# HG_CMAKE_ADD_EXE(HGTestPlugin main.cpp)

# HG_CMAKE_MODULE_DEPEND(
#     TestPlugin
# )

# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/version.h"
# )

# include_directories("${CMAKE_CURRENT_BINARY_DIR}")
