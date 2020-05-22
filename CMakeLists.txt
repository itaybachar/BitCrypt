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
if(WIN32)
  message("Compiling on Windows")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/win)
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
#add_executable(BitCrypt src/main.cpp)
add_executable(BitCrypt src/app/App.cc)

#Link executable
target_link_libraries(BitCrypt App)
target_link_libraries(BitCrypt ${wxWidgets_LIBRARIES})
target_link_libraries(BitCrypt -static-libgcc -static-libstdc++ -luxtheme)
