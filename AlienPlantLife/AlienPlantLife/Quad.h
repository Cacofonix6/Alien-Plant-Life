#ifndef QUAD_H
#define QUAD_H

#include "Triangle.h"

class Quad
{
public: 

	Vertex v[4];

	Quad(glm::vec3 _p[4])
	{		
		glm::vec3 p1[3] = { _p[0], _p[1], _p[2] };
		Triangle triA(p1);

		glm::vec3 p2[3] = { _p[2], _p[3], _p[0] };
		Triangle triB(p2);

		v[0] = triA.v[0];
		v[0].weightedNormal *= 2.0f;
		v[1] = triA.v[1];
		v[1].weightedNormal *= 2.0f;
		v[2] = triA.v[2];
		v[2].weightedNormal *= 2.0f;
		v[3] = triB.v[1];
		v[3].weightedNormal *= 2.0f;
	}
};

#endif