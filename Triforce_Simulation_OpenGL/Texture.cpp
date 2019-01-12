#include "Texture.h"

Texture::Texture(const char * path) : ID(0), data(nullptr), width(0), height(0), numChannels(0)
{
	glGenTextures(1, &ID);

	stbi_set_flip_vertically_on_load(true); // image is upside down by default so lets flip it when we load it

    data = stbi_load(path, &width, &height, &numChannels, 0);
	if (data)
	{
		int format;
		if (numChannels == 1)
		{
			format = GL_RED;
		}	
		else if (numChannels == 3)
		{
			format = GL_RGB;
		}
		else if (numChannels == 4)
		{
			format = GL_RGBA;
		}	

		glBindTexture(GL_TEXTURE_2D, ID);
		std::cout << "Height: " << height << " Width: " << width << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
