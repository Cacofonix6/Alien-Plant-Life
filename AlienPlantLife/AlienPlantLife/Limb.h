#ifndef LIMB_H
#define LIMB_H

#include "Object.h"
#include <stdlib.h> 

struct Segment {

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 origionalDir;
	Segment* parent = nullptr;
	int size = 0;
	int id = 0;
	int count = 0;
	bool hasChild = false;
};

class Limb : public Object
{
public:

	Limb(Segment* _start, int _startId, GLuint _texture);
	~Limb();

	unsigned int getNodeCount() { return nodeCount; }
	int getStartId() { return startId; }

	using Object::draw;

	void createMesh(int _segCount);
	void addSegment(Segment* _seg);

	void draw(Camera _camera, glm::mat4 &_transform);
	void update(Camera _camera, float _growthFactor, glm::mat4 &_transform);

	vector<Transform*> getNodes() { return nodes; }
	vector<Segment*> getSegments() { return segments; }



private:
	vector<Transform*> nodes;
	vector<Segment*> segments;
	float growthFactor;
	unsigned int nodeCount;
	int segCount;
	int startId;
	Shader lineShader;
	glm::vec3 destination;
	GLuint texture;

};


#endif // !LIMB_H
