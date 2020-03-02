#include "Mesh.h"

using namespace std;

////// Constructor //////	
Mesh::Mesh(){}

Mesh::Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures)
{
	this->vertices = _vertices;
	this->indices = _indices;
	this->textures = _textures;

	initMesh();
}
Mesh::~Mesh() {}

////// Functions //////

void Mesh::addTri(glm::vec3 _points[3])
{	
	Triangle tri(_points);

	vector<unsigned int> inds;
	int index = -1;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = vertices.size() - 1; j >= 0; j--)
		{
			if (_points[i] == vertices.at(j).position)
			{
				index = j;
				break;
			}
		}

		if (index == -1)
		{			
			Vertex vert = tri.v[i];
			vertices.push_back(vert);
			inds.push_back(vertices.size() - 1);
		}
		else
		{
			inds.push_back(index);
			vertices.at(index).weightedNormal += tri.v[i].weightedNormal;
			vertices.at(index).normal = glm::normalize(vertices.at(index).weightedNormal);
			index = -1;
		}
	}

	indices.push_back(inds.at(0));
	indices.push_back(inds.at(1));
	indices.push_back(inds.at(2));
}

void Mesh::addTri(glm::vec3 _points[3], unsigned int _index)
{
	Triangle tri(_points);

	vector<unsigned int> inds;
	int index = -1;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = vertices.size() - 1; j >= 0; j--)
		{
			if (_points[i] == vertices.at(j).position)
			{
				index = j;
				break;
			}
		}

		if (index == -1)
		{
			Vertex vert = tri.v[i];
			if (i == 0 )
			{
				vert.texCoords = glm::vec2(_index, 0);
			}
			vertices.push_back(vert);
			inds.push_back(vertices.size() - 1);
		}
		else
		{
			inds.push_back(index);
			vertices.at(index).weightedNormal += tri.v[i].weightedNormal;
			vertices.at(index).normal = glm::normalize(vertices.at(index).weightedNormal);
			index = -1;
		}
		
	}

	
	indices.push_back(inds.at(0));
	indices.push_back(inds.at(1));
	indices.push_back(inds.at(2));
}

void Mesh::addQuad(glm::vec3 _points[4])
{
	Quad quad(_points);

	vector<unsigned int> inds;
	int index = -1;

	
	for (int i = 0; i < 4; ++i)
	{
		
		
		if (vertices.size() != 0)
		{
			for (int j = vertices.size() - 1; j >= 0; j--)
			{
				if (_points[i] == vertices.at(j).position)
				{
					index = j;
					break;
				}
			}
		}

		if (index == -1)
		{
			Vertex vert = quad.v[i];
			vertices.push_back(vert);
			inds.push_back(vertices.size() - 1);
		}
		else
		{
			
			inds.push_back(index);
			vertices.at(index).weightedNormal += quad.v[i].weightedNormal;
			vertices.at(index).normal = glm::normalize(vertices.at(index).weightedNormal);
			index = -1;
		}
	}

	indices.push_back(inds.at(0));
	indices.push_back(inds.at(1));
	indices.push_back(inds.at(2));

	indices.push_back(inds.at(2));
	indices.push_back(inds.at(3));
	indices.push_back(inds.at(0));
}

void Mesh::draw()
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaderID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	if (indices.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_POINTS, 0, vertices.size());
	}
	
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}



void Mesh::initMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
	
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}