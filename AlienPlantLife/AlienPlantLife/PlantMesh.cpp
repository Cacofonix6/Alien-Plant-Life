#include "PlantMesh.h"

////// Constructor //////	
PlantMesh::PlantMesh(GLuint _texture) 
{
	texture = _texture;
}
PlantMesh::~PlantMesh() {}

void PlantMesh::draw() 
{
	
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderID, "texture1"), texture);
	// draw mesh	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, plantVertices.size());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
}

void PlantMesh::initMesh() 
{
	
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, plantVertices.size() * sizeof(PlantVertex), &plantVertices[0], GL_STATIC_DRAW);


	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, position));
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, growthDirection));
	// vertex texture coords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, nextGrowthDirection));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, prevGrowthDirection));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(PlantVertex), (void*)offsetof(PlantVertex, nextSize));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	
}

void PlantMesh::bindTexture(const char* _path)
{
	// load and create a texture 
	// -------------------------
	//unsigned int texture;
	glGenTextures(1, &texture);
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(_path, &width, &height, &nrChannels, 0);
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
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glUniform1i(glGetUniformLocation(shaderID, "texture1"), texture);

}


void PlantMesh::addPoint(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _nextDirection, glm::vec3 _previousDirection, unsigned int _id, float _size, float _nextSize)
{
	PlantVertex vert;
	vert.position = _position;
	vert.growthDirection = _direction;
	vert.nextGrowthDirection = _nextDirection;
	vert.prevGrowthDirection = _previousDirection;
	
	vert.id = (float)_id;
	vert.size = _size;
	vert.nextSize = _nextSize;

	plantVertices.push_back(vert);
}

