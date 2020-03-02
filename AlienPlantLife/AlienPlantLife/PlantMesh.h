#ifndef PLANTMESH_H
#define PLANTMESH_H

#include "Mesh.h"
#include <iostream>

struct PlantVertex {
	float id;
	glm::vec3 position;
	glm::vec3 growthDirection;	
	glm::vec3 nextGrowthDirection;
	glm::vec3 prevGrowthDirection;
	float size;
	float nextSize;
};

class PlantMesh : public Mesh
{
public:
	////// Variables //////
	vector<PlantVertex> plantVertices;

	GLuint texture;

	////// Constructor //////	
	PlantMesh(GLuint _texture);
	~PlantMesh();

	////// Functions //////
	
	void draw();
	void initMesh();
	void bindTexture(const char* _path);


	void addPoint(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _nextDirection, glm::vec3 _previousDirection, unsigned int _id, float _size, float _nextSize);


	//void addTri(glm::vec3 _points[3]);
	//void addTri(glm::vec3 _points[3], unsigned int _index);
	//void addQuad(glm::vec3 _points[4]);
		
};

#endif // !PLANTMESH_H

