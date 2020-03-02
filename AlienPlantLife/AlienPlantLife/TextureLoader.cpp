#include "TextureLoader.h"

TextureLoader::TextureLoader()
{

}
TextureLoader::~TextureLoader()
{
	for (int i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures.at(i)->id);
		delete textures.at(i);
	}
}

GLuint TextureLoader::loadTexture(std::string _path, std::string _name)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures.at(i)->path.compare(_path))
		{
			//return;
		}
	}

	// load and create a texture 
	// -------------------------
	GLuint texture;
	glGenTextures(1, &texture);
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	RawTexture *tex = new RawTexture(texture, _name, _path);

	textures.push_back(tex);

	return texture;
}

GLuint TextureLoader::getTexture(std::string _name)
{
	GLuint out = 0;
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures.at(i)->name.compare(_name))
		{
			out = textures.at(i)->id;
			break;
		}
	}
	return out;
}