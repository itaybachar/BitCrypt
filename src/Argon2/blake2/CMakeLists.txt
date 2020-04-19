#Find Sources
file(GLOB SRCS ./**.cpp ./**.c)

#Library
add_library(Blake2 ${SRCS})

#Define include folder
target_include_directories(Blake2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

