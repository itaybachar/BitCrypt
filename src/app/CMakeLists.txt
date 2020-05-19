#Find Sources
file(GLOB SRCS ./**.cpp ./**.c ./**.cc)

#Library
add_library(App ${SRCS})

#Define include folder
target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

