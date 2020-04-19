#Find Sources
file(GLOB SRCS ./**.cpp ./**.c ./blake2/**.cpp ./blake2/**.c)

#Library
add_library(Argon ${SRCS})

#Define include folder
target_include_directories(Argon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(blake2)

target_link_libraries(Argon Blake2)
