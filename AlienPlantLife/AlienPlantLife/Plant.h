#ifndef PLANT_H
#define PLANT_H

#include "Limb.h"
#include "ParticleEmitter.h"

struct Leaf {
	float id;
	bool reached = false;
	glm::vec3 position;
};

class Plant : public Transform
{
public:

	Plant(GLuint _barkTex, GLuint _leafTex, glm::vec3 _pos, glm::vec3 _volumePos);
	~Plant();

	vector<Leaf*> getLeaves() { return leaves; }


	void update(Camera _camera, float _deltaTime);
	void init();	
	//void getNewLimb(Limb* _limb);
	void grow();
	void createLeaves(); 
	void createSphereVolume(glm::vec3 _position, float _radius, int _count);


private:

	vector<Limb*> limbs;
	vector<Leaf*> leaves;
	vector<ParticleEmitter*> particles;

	const float MIN_DISTANCE = 4;
	const float MAX_DISTANCE = 6;

	int growCount;

	GLuint barkTex;
	GLuint leafTex;

	int highestSegId;
	int segCount;
	float growthFactor;

	glm::vec3 volumePos;

};

#endif // !PLANT_H

