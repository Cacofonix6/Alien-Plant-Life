#ifndef PARTICLE_H
#define PARTICLE_H

#include "Object.h"

class Particle
{
public:

	Particle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation);

	Transform getTransform() { return transform; }
	float getDistance() { return distance; }

	void setParticle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation);

	bool update(float _deltaTime, Camera _camera);

	bool operator<(Particle& _rhs) {
		// Sort in reverse order : far particles drawn first.
		return this->distance > _rhs.distance;
	}

private:
	Transform transform;
	glm::vec3 velocity;
	float gravityEffect;
	float lifeLength;
	float elapsedTime;
	float distance;
};

#endif // !PARTICLE_H
#pragma once
