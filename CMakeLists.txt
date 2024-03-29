# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name
project(bilt)

# Define the source files
set(SOURCE_SERVER src/main_server.cc
                  src/server.cc)
set(SOURCE_CLIENT src/main_client.cc
                  src/client.cc
                  src/database.cc
                  src/camera/camera.cc)

# Find the required libraries
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(OpenCV REQUIRED)

# add ImGui library
add_subdirectory(lib/imgui)

# Build executable for server.cc
add_executable(server ${SOURCE_SERVER})
# Link GLFW to the executable
target_link_libraries(server PRIVATE glfw)
# Link OpenGL to the executable
target_link_libraries(server PRIVATE OpenGL::GL)
# Link ImGui to the executable
target_link_libraries(server PRIVATE imgui)
# Link spdlog to the executable
target_link_libraries(server PRIVATE spdlog::spdlog)
# add header folder
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)
# add ImGui header folder
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# Build executable for client.cc
add_executable(client ${SOURCE_CLIENT})
# Link GLFW to the executable
target_link_libraries(client PRIVATE glfw)
# Link OpenGL to the executable
target_link_libraries(client PRIVATE OpenGL::GL)
# Link ImGui to the executable
target_link_libraries(client PRIVATE imgui)
# Link spdlog to the executable
target_link_libraries(client PRIVATE spdlog::spdlog)
# Link OpenCV to the executable
target_link_libraries(client PRIVATE ${OpenCV_LIBS})
# Link sqlite to the executable
target_link_libraries(client PRIVATE sqlite3)
# Link sqlite to the executable
target_link_libraries(client PRIVATE X11)
# add header folder
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
# add ImGui header folder
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# copy imgui config file
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/build/imgui.ini")
    configure_file("${CMAKE_SOURCE_DIR}/test_data/imgui.ini" "${CMAKE_SOURCE_DIR}/build/imgui.ini" COPYONLY)
endif()

# copy default database file
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/build/test.db")
    configure_file("${CMAKE_SOURCE_DIR}/test_data/test.db" "${CMAKE_SOURCE_DIR}/build/test.db" COPYONLY)
endif()
