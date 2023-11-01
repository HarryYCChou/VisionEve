cmake_minimum_required(VERSION 3.12)
project(imgui)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add imgui source files
add_library(imgui SHARED
  imgui.cpp
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui_impl_glfw.cpp
  imgui_impl_opengl3.cpp
)

# Find the required libraries (GLFW)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Link GLFW to the executable
target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE OpenGL::GL)

# Enable C++17 for the project
set_target_properties(imgui PROPERTIES CXX_STANDARD 17)
set_target_properties(imgui PROPERTIES CXX_STANDARD_REQUIRED ON)
