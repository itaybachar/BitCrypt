cmake_minimum_required(VERSION 3.15)
project(BitCrypt)

#Find wxWidgets
find_package(wxWidgets 3.1 COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

#Set Default Compiler
set(CMAKE_CXX_STANDARD 11)

#Out Directory Logic
list(APPEND ADDITIONAL_LIBS)
list(APPEND CXX_COMPILE_FLAGS)
list(APPEND C_COMPILE_FLAGS)

#list(APPEND C_COMPILE_FLAGS -msha)
add_compile_options(-Wall -Wextra -msha)
if(WIN32)
  message("Compiling on Windows")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/msw)
  list(APPEND ADDITIONAL_LIBS "-luxtheme" "-static-libgcc" "-static-libstdc++")
list(APPEND CXX_COMPILE_FLAGS "-mwindows")
  add_compile_options(-mwindows -pthread)
elseif(APPLE)
  message("Compiling on Apple")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/osx)
else()
  message("Compiling on Unix/Linux")
  add_compile_options(-pthread)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/unx)
  list(APPEND ADDITIONAL_LIBS "-static-libgcc" "-static-libstdc++")
endif()

#Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")

#Link src
add_subdirectory(src)

#Create exe
if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE icon.icns)
  set_source_files_properties(res/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  add_executable(BitCrypt MACOSX_BUNDLE res/icon.icns src/app/App.cc)
else()
  add_executable(BitCrypt src/app/App.cc)
endif()

#Link executable
target_link_libraries(BitCrypt App)
target_link_libraries(BitCrypt ${wxWidgets_LIBRARIES})
target_link_libraries(BitCrypt ${ADDITIONAL_LIBS})

#Install executale
if(APPLE)
  set(CMAKE_INSTALL_PREFIX /Applications)
  install(TARGETS BitCrypt BUNDLE DESTINATION .)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/${APP_NAME}.app")
elseif(NOT WIN32)
  install(TARGETS BitCrypt)  
  install(FILES res/unx/bitcrypt.desktop DESTINATION /usr/share/applications)
  install(FILES res/icon.xpm DESTINATION /usr/share/icons/BitCrypt)
else()
  install(TARGETS BitCrypt)
endif()

#Uninstall Script
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
