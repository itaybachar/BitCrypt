#Find Sources
file(GLOB SRCS ./**.cpp ./**.c)

#Library
add_library(AES ${SRCS})

#Define include folder
target_include_directories(AES PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

