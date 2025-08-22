#pragma once
#include <string>

struct ShdaerProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms;
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
private:
	ShdaerProgramSource ParseShader(const std::string& filepath);
	unsigned int CompilerShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};