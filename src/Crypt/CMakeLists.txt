#Find Sources
file(GLOB SRCS ./**.cpp ./**.c ./**.cc)

#Library
add_library(Crypt ${SRCS})

#Define include folder
target_include_directories(Crypt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

