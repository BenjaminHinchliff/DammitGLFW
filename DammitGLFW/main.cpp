#include "pch.h"

#include <iostream>
#include <array>
#include <cstdio>

#include "Window.h"
#include "shaderLoader.h"
#include "TextureLoader.h"
#include "GlDebug.h"
#include "Camera.h"

// debug defines
#define GL_ERROR_CHECK
//#define CAM_DEBUG

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

// default screen size
constexpr int scrWidth = 800;
constexpr int scrHeight = 600;
constexpr double mouseSensitivity{ 0.001 };

// struct to store camera information
struct GlfwPtrData
{
	Camera cam;
	double deltaTime;
};

int main()
{
	// glfw: initialize and configure
	glfwInit();
	Window::setHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	Window::setHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	Window::setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	Window mainMindow{scrWidth, scrHeight, "OpenGL test"};

	// set callback for when the mouse cursor changes positions
	glfwSetInputMode(mainMindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainMindow, mouseCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	// build and compile the shader program
	Shader lightingShader{"vertex.vert", "fragment.frag" };
	Shader lightShaderProgram{ "vertex.vert", "lightFragment.frag" };

	// set up vertex data and configure vertex attributes
	float vertices[] {
		// positions          // Normal Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// data for the positions of various cubes on the screen
	std::array<glm::vec3, 10> cubePositions {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// BUFFERS
	/*
	generate the VBO (Vertex Buffer Object) that acually store the verticies,
	and the VAO, which stores verticies and associated data to quickly load
	it into memory using glBindVertexArray()
	*/
	GLuint VBO, VAO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind current vertex array so other calls don't affect it
	glBindVertexArray(0);

	// light vertex array generation
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	

	// enable gl depth buffer
	glEnable(GL_DEPTH_TEST);

#ifdef GL_ERROR_CHECK
	// possible error check
	glCheckError();
#endif // GL_ERROR_CHECK

	// MAIN LOOP
	while (!mainMindow.windowShouldClose())
	{
		// clear screen
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update light position
		glm::vec3 lightPosition{ 1.5 * std::cosf(static_cast<float>(glfwGetTime())), 0.0f, 1.5 * std::sinf(static_cast<float>(glfwGetTime())) };

		lightingShader.use();
		lightingShader.setVec3("light.position", lightPosition);

		// light properties
		lightingShader.setVec3("light.ambient", glm::vec3(1.0f));
		lightingShader.setVec3("light.diffuse", glm::vec3(1.0f)); // darken the light a bit to fit the scene
		lightingShader.setVec3("light.specular", glm::vec3(1.0f));

		// material properties
		// material properties
		lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.06f);
		lightingShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		lightingShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		lightingShader.setFloat("material.shininess", 32.0f);

		// set uniforms for all transformations
		// projection matrix
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(
			glm::radians(45.0),
			static_cast<double>(scrWidth) / scrHeight,
			0.1,	// w
			100.0	// dimensions
		);
		lightingShader.setMat4fv("projection", projection);

		// DYNAMIC TRANFORMATIONS

		// view (camera)
		static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
		glm::mat4 view = camera.getViewMatrix();
		lightingShader.setMat4fv("view", view);
		lightingShader.setVec3("viewPos", camera.getPosition());
		
		// pass cam data struct to the window user pointer
		mainMindow.setUserPointer(&camera);

		// draw boxes
		glBindVertexArray(VAO);
		// model translation
		glm::mat4 model{ 1.0f };
		// set model uniform
		lightingShader.setMat4fv("model", model);
		lightingShader.setMat4fv("invViewAndModel", glm::inverse(model * view));

		// render current object
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// also draw the lamp object
		lightShaderProgram.use();
		lightShaderProgram.setMat4fv("projection", projection);
		lightShaderProgram.setMat4fv("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightShaderProgram.setMat4fv("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// input
		processInput(mainMindow);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		mainMindow.swapBuffers();
		glfwPollEvents();
	}
#ifdef GL_ERROR_CHECK
	// possible error check
	glCheckError();
#endif // GL_ERROR_CHECK

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void mouseCallback(GLFWwindow* window, double dXPos, double dYPos)
{
	// get camera object from the glfw window
	Camera* cam{ static_cast<Camera*>(glfwGetWindowUserPointer(window)) };
	// convert x & y pos to floats for the rest of the math
	float xPos = static_cast<float>(dXPos), yPos = static_cast<float>(dYPos);

	static float lastX{ xPos };
	static float lastY{ yPos };

	float xOffset{ xPos - lastX };
	float yOffset{ lastY - yPos };
	lastX = xPos;
	lastY = yPos;
	
	cam->processMouseMovement(xOffset, yOffset, true);
	

#ifdef CAM_DEBUG
	printf("cameraFront(%f, %f, %f)\tYaw: %f\tPitch: %f\n", front.x, front.y, front.z, yaw, pitch);
#endif // CAM_DEBUG
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	// close window if needed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// retrieve camera object and bind local variables
	Camera* cam{ static_cast<Camera*>(glfwGetWindowUserPointer(window)) };

	// get time between frames (delta time)
	static float lastFrame{ static_cast<float>(glfwGetTime()) };

	float deltaTime = static_cast<float>(glfwGetTime()) - lastFrame;
	lastFrame = static_cast<float>(glfwGetTime());

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->processKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->processKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->processKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->processKeyboard(Camera::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cam->processKeyboard(Camera::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cam->processKeyboard(Camera::DOWN, deltaTime);
}