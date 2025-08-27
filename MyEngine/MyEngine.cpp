#include <iostream>
#include <fstream>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int WIDTH = 960;
int HEIGHT = 540;
const char* glsl_version = "#version 460";

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyEngine", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to Create glfwWindow" << std::endl;
		glfwTerminate();//Don't forget
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
			100.0f, 100.0f, 0.0f, 0.0f, //0
			200.0f, 100.0f, 1.0f, 0.0f, //1
			200.0f, 200.0f, 1.0f, 1.0f, //2
			100.0f, 200.0f, 0.0f, 1.0f  //3
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));


		Shader shader("res/shaders/Shader.glsl");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		Texture texture("res/textures/image.png");
		texture.Bind();
		shader.SetUniform1f("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		float r = 0.0f;
		float increment = 0.05f;

		// Our state
		//bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		glm::vec3 translation(200.0f, 200.0f, 0.0f);
		while (!glfwWindowShouldClose(window))
		{
			//input

			//update

			//render
			renderer.Clear();
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * view * model;

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			//if (show_demo_window)
			//	ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				////ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);

				//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

				ImGui::SliderFloat3("translation", &translation.x, 0.0f, 960.0f);   //Model TransLation

				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				//	counter++;
				//ImGui::SameLine();
				//ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			//// 3. Show another simple window.
			//if (show_another_window)
			//{
			//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			//	ImGui::Text("Hello from another window!");
			//	if (ImGui::Button("Close Me"))
			//		show_another_window = false;
			//	ImGui::End();
			//}

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
