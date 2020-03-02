#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Quad.h"
#include "Transform.h"


#include <vector>
#include "stb_image.h"



struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	////// Variables //////	
	vector<Texture> textures;

	////// Constructor //////	
	Mesh();
	Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures);
	~Mesh();

	////// Functions //////
	void addTri(glm::vec3 _points[3]);
	void addTri(glm::vec3 _points[3], unsigned int _index);
	void addQuad(glm::vec3 _points[4]);
	virtual void draw();
	virtual void initMesh();
	
	void setShader(Shader _shader) { shaderID = _shader.getID(); }

protected:
	////// Variables //////
	unsigned int VAO, VBO, EBO, shaderID;

	////// Functions //////
private:
	////// Variables //////	
	vector<Vertex> vertices;
	vector<unsigned int> indices;
};

#endif