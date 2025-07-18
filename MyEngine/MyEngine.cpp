#include <iostream>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/gl.h>

int WIDTH = 800;
int HEIGHT = 600;

void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Failed to init glfw" << std::endl;
		exit(EXIT_FAILURE);
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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



	const std::string vertexShader = "\n"
		"#verson 330\n"
		""
		"\n";

	float vertices[] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(window))
	{
		//input

		//update

		//render
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "Hello CMake." << std::endl;
	return 0;
}
