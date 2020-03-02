#include "Particle.h"

Particle::Particle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation)
{
	transform.lookAt(_lookat);
	transform.rotate(0, 0, _rotation);
	transform.rotate(0, _rotation, 0);
	transform.setPosition(_position);
	transform.setScale(0);
	velocity = _velocity;
	gravityEffect = _gravityEffect;
	lifeLength = _lifeLength;
	elapsedTime = 0;
	distance = -1;
}
void Particle::setParticle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation)
{
	Transform t;
	t.lookAt(_lookat);
	t.rotate(0, 0, _rotation);
	t.rotate(0, _rotation, 0);
	t.setPosition(_position);
	t.setScale(0);
	transform = t;
	transform.lookAt(_lookat);
	velocity = _velocity;
	gravityEffect = _gravityEffect;
	lifeLength = _lifeLength;
	elapsedTime = 0;
	distance = -1;
}

bool Particle::update(float _deltaTime, Camera _camera)
{
	elapsedTime += _deltaTime;

	

	if (elapsedTime < lifeLength * 0.95f)
	{
		float scale = elapsedTime / lifeLength;

		transform.setScale(scale * 0.5f, scale * 0.5f, scale * 0.5f);
		transform.setPosition(transform.getPosition() + (transform.getUp() * 0.01f * _deltaTime));
		//cout << transform.getScale() << endl;
	}
	else
	{
		float gravity = -9.81;
		velocity.y += gravity * gravityEffect * _deltaTime;
		glm::vec3 change = velocity;
		change *= _deltaTime;
		transform.setPosition(transform.getPosition() + change);
	}
	
	distance = glm::length2(transform.getPosition() - _camera.Position);
	return elapsedTime < lifeLength;
}