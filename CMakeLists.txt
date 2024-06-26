cmake_minimum_required(VERSION 3.14)

# Set project name
project(meh
        VERSION 0.1
        DESCRIPTION "A lightweight model viewer and 3MF packager"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# pmp
add_subdirectory(extern/pmp-library)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/src/)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/eigen)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/glew/include)

# File Dialog for ImGui
include_directories(${PROJECT_SOURCE_DIR}/extern/ImGuiFileDialog)

# add source utility
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add library directory
add_subdirectory(src)

# Add executable Directory
add_subdirectory(apps)

# Add UI Directory
add_subdirectory(ui)
