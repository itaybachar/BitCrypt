#Find Sources
file(GLOB SRCS ./**.cc ./**.c)

#Library
add_library(Util ${SRCS})

#Define include folder
target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

