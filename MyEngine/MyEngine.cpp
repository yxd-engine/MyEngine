#include <iostream>
#include <fstream>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

int WIDTH = 800;
int HEIGHT = 600;

struct  ShdaerProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShdaerProgramSource ParseShdaer(const std::string& filepath)
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

static unsigned int CompilerShader(unsigned int type, const std::string& source)
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


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Failed to init glfw" << std::endl;
		exit(EXIT_FAILURE);
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyEngine", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to Create glfwWindow" << std::endl;
		glfwTerminate();//Don't Remember
		exit(EXIT_FAILURE);
		return -1;
	}


	//window cneter
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(window, mode->width / 2 - WIDTH / 2, mode->height / 2 - HEIGHT / 2);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		//此用于域是因为  vertexbuffer和indexbuffer 是栈中分配，当glfw销毁opengl context的时候，vbo和ibo还存在，所以当vbo和ibo对象销毁会一直有问题
		float vertices[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,

		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		unsigned int VAO;
		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glBindVertexArray(VAO));

		VertexBuffer vbo(vertices, sizeof(float) * 4 * 2);

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

		IndexBuffer ibo(indices, 6);

		ShdaerProgramSource source = ParseShdaer("res/shaders/Shader.glsl");

		unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(program));

		GLCall(unsigned int location = glGetUniformLocation(program, "u_Color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));


		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		float r = 0.0f;
		float increment = 0.05f;
		while (!glfwWindowShouldClose(window))
		{
			//input

			//update

			//render
			glClear(GL_COLOR_BUFFER_BIT);

			GLCall(glUseProgram(program));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

			GLCall(glBindVertexArray(VAO));
			ibo.Bind();
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}
			r += increment;

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		GLCall(glDeleteProgram(program));
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
