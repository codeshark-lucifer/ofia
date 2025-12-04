#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>

// Global helper function (should ideally be in an anonymous namespace or static in a cpp)
inline void _CheckGLError(const std::string& file, int line, const std::string& call) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "[OpenGL Error] " << file << ":" << line << " (" << call << "): " << error << std::endl;
    }
}

#define GL_CHECK(call) do { call; _CheckGLError(__FILE__, __LINE__, #call); } while (false)
