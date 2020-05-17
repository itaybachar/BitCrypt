#Add Subdirectories
add_subdirectory(Argon2)
add_subdirectory(AES)
add_subdirectory(Util)
add_subdirectory(Crypt)


target_link_libraries(Crypt Argon)
target_link_libraries(Crypt AES)
target_link_libraries(Crypt Util)

#Create exe
add_executable(BitCrypt main.cpp)

#Link executable
target_link_libraries(BitCrypt Crypt)
