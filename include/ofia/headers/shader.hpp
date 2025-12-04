#ifndef OFIA_SHADER_HPP
#define OFIA_SHADER_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace ofia
{
    class Shader
    {
    public:
        unsigned int ID;

        // ---------------- Constructors ----------------
        Shader() : ID(0) {}

        // GLSL single file with #shader blocks
        Shader(const std::string &filepath);

        // Separate vertex & fragment files
        Shader(const std::string &vertexPath, const std::string &fragmentPath);

        ~Shader();

        // ---------------- Use Program ----------------
        void Use() const;

        // ---------------- Uniform Setters ----------------
        void SetUniform(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
        void SetUniform(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
        void SetUniform(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }

        void SetUniform(const std::string &name, const glm::vec2 &v) const { glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v)); }
        void SetUniform(const std::string &name, const glm::vec3 &v) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v)); }
        void SetUniform(const std::string &name, const glm::vec4 &v) const { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v)); }

        void SetUniform(const std::string &name, const glm::mat3 &m) const { glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m)); }
        void SetUniform(const std::string &name, const glm::mat4 &m) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m)); }

    private:
        void CompileShaders(const std::string &vertexSrc, const std::string &fragmentSrc);
        std::string LoadFile(const std::string &path);
    };

}

#endif // OFIA_SHADER_HPP