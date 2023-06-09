#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Game.h"
#include "Engine/ResourceManager.h"

#include "Constants/Screen.h"

Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT);

void keyCallback(GLFWwindow*, int key, int, int action, int)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			game.keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			game.keys[key] = false;
		}
	}
}

void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "IT Technician", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game.Init();

	double deltaTime = 0.0;
	double lastFrame = 0.0;

	while (!glfwWindowShouldClose(window) && game.isRunning)
	{
		glfwPollEvents();
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.ProcessInput();
		game.Update(deltaTime);

		game.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}