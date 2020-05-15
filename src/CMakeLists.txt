#Add Subdirectories
add_subdirectory(Argon2)
add_subdirectory(AES)

#Link Libraries

#Create exe
add_executable(BitCrypt main.cpp)

#Link executable
target_link_libraries(BitCrypt Argon)
target_link_libraries(BitCrypt AES)
