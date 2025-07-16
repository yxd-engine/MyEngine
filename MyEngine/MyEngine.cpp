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


	while (!glfwWindowShouldClose(window))
	{
		//input

		//update

		//render

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "Hello CMake." << std::endl;
	return 0;
}
