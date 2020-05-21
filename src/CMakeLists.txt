#Add Subdirectories
add_subdirectory(Argon2)
add_subdirectory(AES)
add_subdirectory(Util)
add_subdirectory(Crypt)
add_subdirectory(app)

target_link_libraries(Crypt Argon)
target_link_libraries(Crypt AES)
target_link_libraries(Crypt Util)
target_link_libraries(App Crypt)


