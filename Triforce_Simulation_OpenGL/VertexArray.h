#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct VertexArrayAttribute
{
	unsigned int count;
	unsigned int type;
	unsigned int normalized; // int because of the boolean is just a 0 or 1
	unsigned int stride;
	unsigned int offset;

	//Help initalize stuff for me in order to add the object
	VertexArrayAttribute(unsigned int count, unsigned int type, unsigned int normalized, unsigned int stride, unsigned int offset) :
		count(count), type(type), normalized(normalized), stride(stride), offset(offset) 
	{}
};

class VertexArray
{
public:
	VertexArray(std::vector<VertexArrayAttribute> data);
	~VertexArray();

	void bind();
	void unbind();

	unsigned int getTypeSize(unsigned int type);
private: 
	unsigned int ID;
	
	std::vector<VertexArrayAttribute> attributes;
};
