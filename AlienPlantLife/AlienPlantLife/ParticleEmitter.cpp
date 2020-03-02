#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(float _pps, glm::vec3 _ptA, glm::vec3 _ptB, GLuint _texture, int _id) :
	shader("resources/Shaders/Particle.vert", 
		"resources/Shaders/Particle.frag",
		"resources/Shaders/Particle.geom")
{
	this->init();
	pps = _pps;
	frameCount = 0;
	positionA = _ptA;
	positionB = _ptB;
	srand(time(NULL));
	//loadTexture("resources/Textures/Leaf2.png");
	texture = _texture;
	id = _id;
}

ParticleEmitter::~ParticleEmitter()
{
	for (int i = 0; i < aliveParticles.size(); i++)
	{
		delete aliveParticles.at(i);
	}
	for (int i = 0; i < deadParticles.size(); i++)
	{
		delete deadParticles.at(i);
	}
}

void ParticleEmitter::init()
{	
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	
	
	glGenBuffers(1, &this->VBO);
	
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	
	
}

void ParticleEmitter::update(GLfloat dt, Camera _camera)
{
	float emitNumber = pps * dt;

	if (frameCount >= 1)
	{
		distributeParticles();
		frameCount = 0;
	}
	else
	{
		frameCount += emitNumber;
	}


	//cout << dt << endl;
	for (int i = aliveParticles.size() - 1; i >= 0; i--)
	{
		Particle* p = aliveParticles.at(i);

		if (!p->update(dt, _camera))
		{
			aliveParticles.erase(aliveParticles.begin() + i);
			deadParticles.push_back(p);
		}
	}

	std::sort(aliveParticles.begin(), aliveParticles.end());
}

void ParticleEmitter::addParticle(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _lookat, float _gravityEffect, float _lifeLength, float _rotation)
{
	if (deadParticles.size() > 0)
	{
		Particle *p = deadParticles.at(0);
		deadParticles.erase(deadParticles.begin());
		p->setParticle(_position, _velocity, _lookat, _gravityEffect, _lifeLength, _rotation);
		aliveParticles.push_back(p);
	}
	else
	{
		Particle *p = new Particle(_position, _velocity, _lookat, _gravityEffect, _lifeLength, _rotation);
		aliveParticles.push_back(p);
	}

}

float randomRange(float _min, float _max)
{
	float perc = rand() % 100;
	perc /= 100.0f;

	float range = _max - _min;
	//perc *= range;

	return (perc * range) + _min;
}

void ParticleEmitter::distributeParticles()
{

	float xVel = randomRange(-3, 3); 
	float yVel = randomRange(-2, -1);
	float zVel = randomRange(-3, 3);
	float life = randomRange(20, 50);

	float rot = randomRange(0, 360);

	float offsetScalar = randomRange(0, 1);
	glm::vec3 offset = positionB - positionA;
	offset *= offsetScalar;

	glm::vec3 lookat = glm::normalize(positionB - positionA) * 3.0f;

	//cout << xVel << ", " << zVel << endl;

	addParticle(positionA + offset, glm::vec3(0, yVel, 0), lookat, 0.0f, life, rot);
}


void ParticleEmitter::render(Camera _camera, glm::mat4 &_tranform)
{

	glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom), (float)Window::WIDTH / (float)Window::HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = _camera.GetViewMatrix();

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setInt("particleTex", texture);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
	glActiveTexture(GL_TEXTURE0 + texture);

	for (Particle *particle : aliveParticles)
	{		
		glBindTexture(GL_TEXTURE_2D, texture);
		updateModelMatrix(particle->getTransform(), _tranform);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	glActiveTexture(GL_TEXTURE0);
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void ParticleEmitter::updateModelMatrix(Transform _transform, glm::mat4 &_parentMatrix, glm::mat4 &_viewMatrix)
{
	glm::mat4 model = _parentMatrix * _transform.getTranformMatrix() ;
	
	shader.setMat4("model", model);
}

