#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader
{
private:
	GLuint m_ID;

public:
	// read from file and set up shaders
	Shader(const char *vertexPath, const char *fragmentPath);

	// activate shader
	void use();
	// uniform setting functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4fv(const std::string &name, const glm::mat4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, const float x, const float y, const float z) const;
	// duplicate of glActiveTexture(), but I think this is more intuitive
	static void setActiveTexture(int texNum);

	const GLuint& getID() const;
};