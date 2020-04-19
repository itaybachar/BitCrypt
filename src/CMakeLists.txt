#Add Subdirectories
add_subdirectory(Argon2)
#add_subdirectory(AES)

#Link Libraries
#target_link_libraries(AES HashTools)

#Create exe
add_executable(BitCrypt main.cpp)

#Link executable
target_link_libraries(BitCrypt Argon)
