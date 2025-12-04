cmake_minimum_required(VERSION 3.10)
project(OfiaEngine VERSION 0.1 LANGUAGES C CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/glad)
include_directories(${IMGUI_DIR})
include_directories(${LIB_DIR}) # For external headers if needed

# Source files
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/utils/glad.c
    ${SRC_DIR}/utils/stb_image.cpp

    ${SRC_DIR}/application.cpp
    ${SRC_DIR}/core/platform.cpp

    ${SRC_DIR}/core/shader.cpp
    ${SRC_DIR}/core/camera.cpp
    ${SRC_DIR}/core/texture.cpp
    ${SRC_DIR}/core/scene.cpp
    ${SRC_DIR}/core/transform.cpp

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add executable
add_executable(engine ${SOURCES})

# Include directories for target
target_include_directories(engine PRIVATE ${INCLUDE_DIR} ${IMGUI_DIR} ${INCLUDE_DIR}/stb ${INCLUDE_DIR}/assimp)

# Find and link GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
if(GLFW_FOUND)
    target_include_directories(engine PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(engine PRIVATE ${GLFW_LIBRARIES})
else()
    message(FATAL_ERROR "GLFW not found")
endif()

find_package(OpenGL REQUIRED)

# Link Windows system libs
if(WIN32)
    target_link_libraries(engine PRIVATE
        ${OPENGL_LIBRARIES}
        opengl32
        gdi32
        user32
        kernel32
    )
endif()

# ---- ASSIMP LINKING ----
set(ASSIMP_LIB "${LIB_DIR}/libassimp.dll.a")
set(ASSIMP_DLL "${LIB_DIR}/libassimp-6.dll")

if(EXISTS ${ASSIMP_LIB})
    target_link_libraries(engine PRIVATE ${ASSIMP_LIB})
endif()

# Output directory
set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy Assimp DLL after build
if(EXISTS ${ASSIMP_DLL})
    add_custom_command(TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSIMP_DLL}"
            "$<TARGET_FILE_DIR:engine>"
    )
endif()
