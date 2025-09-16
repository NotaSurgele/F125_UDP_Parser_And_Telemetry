cmake_minimum_required(VERSION 3.16)

# Project name and version
project(F1_UDP_Parser VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Asio
include(FetchContent)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-36-0
)
FetchContent_MakeAvailable(asio)

# Fetch ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.2
        GIT_SHALLOW OFF
)
FetchContent_MakeAvailable(imgui)

# Add executable
add_executable(${PROJECT_NAME}
        src/main.cpp

        # Core ImGui files
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp

        # ImGui backends
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp

        # Your headers (optional)
        include/receiver.h
        src/receiver.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${asio_SOURCE_DIR}/asio/include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Link system libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 opengl32 dwmapi)
endif()

# Optional: add compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -g -g3)
endif()
