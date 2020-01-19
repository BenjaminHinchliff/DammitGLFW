#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory> // for std::unique_ptr<>()
#include <string> // for std::string

using uCharPtr_t = unsigned char*;

class Image
{
private:
	const uCharPtr_t m_img = nullptr;
	int m_width{};
	int m_height{};
	int m_nrChannels{};

public:
	// constructors
	Image(std::string path);

	~Image();

	// getters
	const uCharPtr_t getImg() const;
	int getWidth() const;
	int getHeight() const;
	int getNrChannels() const;
};