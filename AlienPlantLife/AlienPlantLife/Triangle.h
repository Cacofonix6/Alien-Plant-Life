#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 weightedNormal;
};

class Triangle
{
public :
	Vertex v[3];

	//         c
	//         ^
	//         |
	//         |
	//  a----->b

	Triangle(glm::vec3 _p[3])
	{
		v[0].position = _p[0];
		v[1].position = _p[1];
		v[2].position = _p[2];

		glm::vec3 wNorm = glm::cross((v[1].position - v[0].position), (v[2].position - v[0].position));
		v[0].weightedNormal = wNorm;
		v[1].weightedNormal = wNorm;
		v[2].weightedNormal = wNorm;

		glm::vec3 norm = glm::normalize(wNorm);
		v[0].normal = norm;
		v[1].normal = norm;
		v[2].normal = norm;

		glm::vec3 zero3(0, 0, 0);
		v[0].tangent = zero3;
		v[1].tangent = zero3;
		v[2].tangent = zero3;

		v[0].bitangent = zero3;
		v[1].bitangent = zero3;
		v[2].bitangent = zero3;

		glm::vec2 zero2(0, 0);
		v[0].texCoords = zero2;
		v[1].texCoords = zero2;
		v[2].texCoords = zero2;
	}

};

#endif
