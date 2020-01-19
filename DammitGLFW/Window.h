#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

/*
* class to abstract away window stuff
* Note:
* still need to call glfwInit & glfwTerminate functions manually
*/
class Window
{
private:
	GLFWwindow *m_window;

private:
	static void m_framebufferSizeCallback(GLFWwindow *m_window, int width, int height);

public:
	Window(int width, int height, const std::string &title);

	static void setHint(int hint, int value) { glfwWindowHint(hint, value); }
	void swapBuffers() { glfwSwapBuffers(m_window); }
	void pollEvents() { glfwPollEvents(); }
	void setUserPointer(void* ptr) { glfwSetWindowUserPointer(m_window, ptr); }
	void* getUserPointer() { return glfwGetWindowUserPointer(m_window); }
	GLFWwindow* getWindow() { return m_window; }
	bool windowShouldClose() { return glfwWindowShouldClose(m_window); }
	
	// window cast function
	operator GLFWwindow*() { return m_window; }
};