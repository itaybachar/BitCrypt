cmake_minimum_required(VERSION 3.15)
project(BitCrypt)

#Find wxWidgets
find_package(wxWidgets 3.1 REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

#Set Default Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msha -msse4.1 -mssse3 -Wpsabi -pthread ${CMAKE_CXX_FLAGS_DEBUG}")
#Needed to compile argon
#set(CMAKE_C_FLAGS "-m32")

#Link src
add_subdirectory(src)

#Create exe
#add_executable(BitCrypt main.cpp)
add_executable(BitCrypt src/app/App.cc)

#Link executable
target_link_libraries(BitCrypt App)
target_link_libraries(BitCrypt ${wxWidgets_LIBRARIES})
target_link_libraries(BitCrypt -static-libgcc -static-libstdc++)
