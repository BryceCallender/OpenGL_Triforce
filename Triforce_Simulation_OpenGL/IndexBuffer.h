#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();
private:
	unsigned int ID;
	unsigned int count;
};

