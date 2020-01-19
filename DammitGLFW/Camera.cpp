#include "Camera.h"

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front
	{
		cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),	// x
		sin(glm::radians(m_pitch)),							// y
		sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))	// z
	};
	m_front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_movementSpeed * deltaTime;
	switch (direction)
	{
	case Camera::FORWARD:
		m_position += m_front * velocity;
		break;
	case Camera::BACKWARD:
		m_position -= m_front * velocity;
		break;
	case Camera::LEFT:
		m_position -= m_right * velocity;
		break;
	case Camera::RIGHT:
		m_position += m_right * velocity;
		break;
	case Camera::UP:
		m_position += m_up * velocity;
		break;
	case Camera::DOWN:
		m_position -= m_up * velocity;
		break;
	default:
		throw "Invalid Direction";
		break;
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}