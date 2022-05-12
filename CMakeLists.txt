cmake_minimum_required(VERSION 3.14)

# Set project name
project(meh 
        VERSION 0.1
        DESCRIPTION "A lightweight model viewer and 3MF packager"
        LANGUAGES CXX)

set(BUILD_WIN TRUE)

if(${BUILD_WIN})
        message("Setting build settings for windows, compiler options")
        set(CMAKE_SYSTEM_NAME Windows)

        SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
        SET(CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++)
        SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
        set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")
        set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
endif()

# pmp
add_subdirectory(extern/pmp-library)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/src)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/eigen)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/extern/pmp-library/external/glew/include)

# File Dialog for ImGui
add_subdirectory(extern/ImGuiFileDialog)
include_directories(${PROJECT_SOURCE_DIR}/extern/ImGuiFileDialog)

# add source utility
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add library directory
add_subdirectory(src)

# Add executable Directory
add_subdirectory(apps)

# Add UI Directory
add_subdirectory(ui)
