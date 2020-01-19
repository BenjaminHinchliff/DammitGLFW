#include "imgLoad.h"
// This has to be here because of how STBI is implememted.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(std::string path)
	: m_img{ stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0) }
{
}

Image::~Image()
{
	stbi_image_free(m_img);
}

const uCharPtr_t Image::getImg() const { return m_img; }
int Image::getWidth() const { return m_width; }
int	Image::getHeight() const { return m_height; }
int Image::getNrChannels() const { return m_nrChannels; }