cmake_minimum_required(VERSION 3.15)
project(BitCrypt)

#Find wxWidgets
find_package(wxWidgets 3.1 COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

#Set Default Compiler
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msha -pthread ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msha -pthread ${CMAKE_CXX_FLAGS_RELEASE}")

#Out Directory Logic
list(APPEND ADDITIONAL_LIBS)
if(WIN32)
  message("Compiling on Windows")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/msw)
  list(APPEND ADDITIONAL_LIBS "-luxtheme")
elseif(APPLE)
  message("Compiling on Apple")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/osx)
else()
  message("Compiling on Unix/Linux")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/unx)
endif()

#Link src
add_subdirectory(src)

#Create exe
add_executable(BitCrypt src/app/App.cc)

#Link executable
target_link_libraries(BitCrypt App)
target_link_libraries(BitCrypt ${wxWidgets_LIBRARIES})
target_link_libraries(BitCrypt -static-libgcc -static-libstdc++ ${ADDITIONAL_LIBS})


#Install executale
install(TARGETS BitCrypt)
if(NOT WIN32)
  install(FILES res/unx/bitcrypt.desktop DESTINATION /usr/share/applications)
  install(FILES res/icon.xpm DESTINATION /usr/share/icons/BitCrypt)
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

