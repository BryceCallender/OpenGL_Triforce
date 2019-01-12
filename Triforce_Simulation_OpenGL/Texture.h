#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

private: 
	unsigned int ID;
	unsigned char* data;
	int width, height, numChannels;
};

