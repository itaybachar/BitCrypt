#Find Sources
file(GLOB SRCS ./**.cc ./**.c)

#Library
add_library(App ${SRCS})

#Define include folder
target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(App PUBLIC ${CMAKE_SOURCE_DIR}/res)

