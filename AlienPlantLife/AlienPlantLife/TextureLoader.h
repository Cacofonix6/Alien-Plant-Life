#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <string>

struct RawTexture
{
	GLuint id;
	std::string name;
	std::string path;

	RawTexture(GLuint _id, std::string _name, std::string _path)
	{
		id = _id;
		name = _name;
		path = _path;
	}
};

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	GLuint loadTexture(std::string _path, std::string _name);
	GLuint getTexture(std::string _name);

private:
	std::vector<RawTexture*> textures;
};

#endif // !TEXTURE_LOADER_H

