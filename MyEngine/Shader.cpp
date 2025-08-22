#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath) :m_FilePath(filepath), m_RendererID(0)
{

}

Shader::~Shader()
{

}


ShdaerProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShdaerType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	std::string line;
	ShdaerType type = ShdaerType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShdaerType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShdaerType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };

}

unsigned int Shader::CompilerShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// Error Handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//char* message = (char*)alloca(length * sizeof(char));//warning stackoverflow
		std::string message(length, '\0');
		GLCall(glGetShaderInfoLog(id, length, &length, message.data()));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompilerShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void Shader::Bind() const
{

}

void Shader::Unbind() const
{

}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3)
{

}


unsigned int Shader::GetUniformLocation(const std::string& name)
{

}
