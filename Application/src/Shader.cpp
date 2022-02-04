#include "Shader.h"
#include "Core.h"

#include <iostream>
#include <sstream>
#include <fstream>


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererId(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererId = CreateShader(source.vertex, source.fragment);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererId));
}

ShaderProgramSource Shader::ParseShader(const std::string& path)
{
    std::ifstream stream(path);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str() , ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    // creates the shader ID from the shader type and compiles from shader source
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // handdles errors thrown by the shader compiler
    // only active in DEBUG mode
#ifdef DEBUG
    int resault;
    glGetShaderiv(id, GL_COMPILE_STATUS, &resault);
    if (resault == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char(length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed To compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";
        delete message;
        glDeleteShader(id);
        return 0;
    }
#endif
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // creates the variable to hold the adress to the shader program
    unsigned int program = glCreateProgram();
    // compiles the shaders from provided source code
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attatches links and compiles shader programes together to run
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    // validates the program so the shader runs proporly
    GLCall(glValidateProgram(program));

    // deletes intermediates to save on space
    glDeleteShader(vs);
    glDeleteShader(fs);
    // returns the completed shader program
    return program;
};

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererId));
}
void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
 
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}