# Install script for directory: D:/virtualMachine/github/vsproject/HGAppModule/HGExceptionHandleModule

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/HGAppModule")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "D:/Qt/Tools/mingw1310_64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib/libHGExceptionHandleModule.dll.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/virtualMachine/github/vsproject/HGAppModule/build_windows/HGExceptionHandleModule/libHGExceptionHandleModule.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib/libHGExceptionHandleModule.dll")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib" TYPE SHARED_LIBRARY FILES "D:/virtualMachine/github/vsproject/HGAppModule/build_windows/HGExceptionHandleModule/libHGExceptionHandleModule.dll")
  if(EXISTS "$ENV{DESTDIR}/D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib/libHGExceptionHandleModule.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib/libHGExceptionHandleModule.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "D:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}/D:/virtualMachine/github/vsproject/HGAppModule/hg_pub/win32/lib/libHGExceptionHandleModule.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/virtualMachine/github/vsproject/HGAppModule/build_windows/HGExceptionHandleModule/CMakeFiles/HGExceptionHandleModule.dir/install-cxx-module-bmi-Release.cmake" OPTIONAL)
endif()

