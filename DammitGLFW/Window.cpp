#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "Window.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Window::m_framebufferSizeCallback(GLFWwindow *m_window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const std::string &title)
	: m_window{ glfwCreateWindow(width, height, title.c_str(), NULL, NULL) }
{
	if (!m_window)
	{
		glfwTerminate();
		throw "Failed to create GLFW window";
	}
	else
	{
		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, m_framebufferSizeCallback);
	}
}