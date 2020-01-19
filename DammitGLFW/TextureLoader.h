#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <cstdio>

class Texture
{
private:
	GLuint m_ID;
	const GLenum m_wrap_s, m_wrap_t;
	const GLenum m_min_filter, m_mag_filter;

public:
	Texture(
		std::string path,
		GLenum wrapS = GL_REPEAT,
		GLenum wrapT = GL_REPEAT,
		GLenum minFilter = GL_LINEAR,
		GLenum magFilter = GL_LINEAR
	);

	const GLuint& getID() { return m_ID; }
	void bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); }
};