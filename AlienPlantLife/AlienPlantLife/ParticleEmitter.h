#ifndef PARTICLEEMTITTER_H
#define PARTICLEEMTITTER_H

#include "Particle.h"
#include <time.h>

static const GLfloat vertices[] = {
	0.0f, 0.0f, 0.0f 
};

class ParticleEmitter
{
public:
	ParticleEmitter(float _pps, glm::vec3 _ptA, glm::vec3 _ptB, GLuint _texture, int _id);
	~ParticleEmitter();

	void update(GLfloat dt, Camera _camera);
	void render(Camera _camera, glm::mat4 &_tranform);
	void addParticle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation);
	void distributeParticles();

	int getId() { return id; }

private:
	// State
	glm::vec3 positionA;
	glm::vec3 positionB;
	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;
	float pps;
	float frameCount;
	GLuint amount;
	int id;
	// Render state
	Shader shader;
	//Texture2D texture;
	GLuint VAO, VBO;
	// Initializes buffer and vertex attributes
	void init();


	unsigned int texture;
	void updateModelMatrix(Transform _transform, glm::mat4 &_parentMatrix, glm::mat4 &_viewMatrix = glm::mat4(0));
	
};

#endif // !PARTICLEEMTITTER_H

