#include "ofia/headers/shader.hpp"
#include <fstream>
#include <sstream>
namespace ofia
{
    Shader::Shader(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << filepath << std::endl;
            return;
        }

        std::stringstream ss[2];
        enum class Type
        {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1
        } type = Type::NONE;
        std::string line;

        while (std::getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = Type::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = Type::FRAGMENT;
            }
            else if (type != Type::NONE)
            {
                ss[(int)type] << line << '\n';
            }
        }

        CompileShaders(ss[0].str(), ss[1].str());
    }

    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    {
        std::string vertexSrc = LoadFile(vertexPath);
        std::string fragmentSrc = LoadFile(fragmentPath);

        CompileShaders(vertexSrc, fragmentSrc);
    }

    std::string Shader::LoadFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << path << std::endl;
            return "";
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    void Shader::CompileShaders(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSrc = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vSrc, nullptr);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSrc = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fSrc, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::Use() const
    {
        if (ID != 0)
            glUseProgram(ID);
    }

    Shader::~Shader()
    {
        if (ID != 0)
            glDeleteProgram(ID);
    }

} // namespace ofia
