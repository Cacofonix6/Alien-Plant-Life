
#include "Limb.h"

#include <ctime>
//Plant::Plant() : Object("resources/Shaders/PlantShaderPoint.vert", "resources/Shaders/PlantShaderPoint.frag", "resources/Shaders/PlantShaderPoint.geom")
Limb::Limb(Segment* _start, int _startId, GLuint _texture) : Object("resources/Shaders/PlantShader.vert", "resources/Shaders/PlantShader.frag", "resources/Shaders/PlantShader.geom")
, lineShader("resources/Shaders/PlantShaderLine.vert", "resources/Shaders/PlantShaderLine.frag", "resources/Shaders/PlantShaderLine.geom")
{
	growthFactor = 0.01f;
	segments.push_back(_start);
	startId = _startId;
	texture = _texture;
}

Limb::~Limb()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes.at(i);
	}

	for (int i = 0; i < segments.size(); i++)
	{
		delete segments.at(i);
	}
}

void Limb::draw(Camera _camera, glm::mat4 &_transform)
{
	shader.use();

	glm::mat4 projectionMat = glm::perspective(glm::radians(_camera.Zoom), (float)Window::WIDTH / (float)Window::HEIGHT, 0.1f, 100.0f);
	glm::mat4 viewMat = _camera.GetViewMatrix();

	shader.setFloat("growthFactor", growthFactor);
	shader.setFloat("mutator", segCount);

	glm::mat4 world = projectionMat * viewMat * _transform;
	shader.setMat4("world", world);
	shader.setMat4("model", _transform);
	shader.setVec3("LightPos", glm::vec3(0, 20, 0));

	model.draw(shader);

	lineShader.use();
	lineShader.setMat4("world", world);
	
	//model.draw(lineShader);
}

void Limb::update(Camera _camera, float _growthFactor, glm::mat4 &_transform)
{
	growthFactor = _growthFactor;
	
	draw(_camera,_transform);
}

void Limb::addSegment(Segment* _seg)
{
	segments.push_back(_seg);
}

void Limb::createMesh(int _segCount)
{
	PlantMesh* mesh = new PlantMesh(texture);

	segCount = _segCount;
	
	for (int i = 0; i < segments.size(); i++)
	{
		Segment* seg = segments.at(i);

		if (segments.size() == 1)
		{
			if (seg->parent != nullptr)
			{
				mesh->addPoint(segments.at(i)->parent->position, segments.at(i)->direction, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), seg->id, seg->size, 0.0f);
			}
		}
		else if (i == 0)
		{
			if (seg->parent != nullptr)
			{
				mesh->addPoint(segments.at(i)->parent->position, segments.at(i)->direction, segments.at(i + 1)->direction, glm::vec3(0, 0, 0), seg->id, seg->size, segments.at(i + 1)->size);
			}
		}
		else if (i == segments.size() - 1)
		{
			mesh->addPoint(segments.at(i)->parent->position, segments.at(i)->direction, glm::vec3(0, 0, 0), segments.at(i - 1)->direction, seg->id, seg->size, 0.0f);
		}
		else
		{
			mesh->addPoint(segments.at(i)->parent->position, segments.at(i)->direction, segments.at(i + 1)->direction, segments.at(i - 1)->direction, seg->id, seg->size, segments.at(i + 1)->size);
		}
	}
	mesh->initMesh();
	addMesh(mesh);
}
