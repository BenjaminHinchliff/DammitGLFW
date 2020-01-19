#include "pch.h"
#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) :
	m_wrap_s{ wrapS }, m_wrap_t{ wrapT }, m_min_filter{ minFilter }, m_mag_filter{ magFilter }
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
	// flip right side up
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum channels{ static_cast<GLenum>(nrChannels == 3 ? GL_RGB : GL_RGBA) };
		glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw "Failed to load texture";
	}
	stbi_image_free(data);
}