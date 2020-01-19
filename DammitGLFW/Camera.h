#pragma once

#include "pch.h"

#include <vector>

// Default camera values
constexpr glm::vec3 defaultPos{ 0.0f, 0.0f, 0.0f };
constexpr glm::vec3 defaultFront{ 0.0f, 0.0f, -1.0f };
constexpr glm::vec3 defaultWorldUp{ 0.0f, 1.0f, 0.0f };
constexpr float defaultYaw{ -90.0f };
constexpr float defaultPitch{ 0.0f };
constexpr float defaultSpeed{ 2.5f };
constexpr float defaultSensitivity{ 0.1f };
constexpr float defaultZoom{ 45.0f };

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
// Note: glm by default can only really work with floats rather that doubles. I don't really know why. So that's why they're used.
class Camera
{
public:
	// camera enum to allow use of more intuitive movement directions
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

private:
	// Camera Attributes
	glm::vec3 m_position = defaultPos;
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp = defaultWorldUp;
	// Euler Angles
	float m_yaw = defaultYaw;
	float m_pitch = defaultPitch;
	// Camera options
	float m_movementSpeed = defaultSpeed;
	float m_mouseSensitivity = defaultSensitivity;
	float m_zoom = defaultZoom;

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

public:
	// Constructor with vectors
	Camera(glm::vec3 position = defaultPos, glm::vec3 worldUp = defaultWorldUp, float yaw = defaultYaw, float pitch = defaultPitch) :
		m_position{ position }, m_worldUp{ worldUp }, m_yaw{ yaw }, m_pitch{ pitch }
	{
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix(); 

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);

	// Setter and getter funcitons
	const glm::vec3& getPosition() { return m_position; }
};