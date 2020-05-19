#Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

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

#Create exe
#add_executable(BitCrypt main.cpp)
add_executable(BitCrypt app/App.cc)

#Link executable
target_link_libraries(BitCrypt App)
target_link_libraries(BitCrypt ${wxWidgets_LIBRARIES})
