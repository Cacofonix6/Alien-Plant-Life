#include "Plant.h"
#include <ctime>

Plant::Plant(GLuint _barkTex, GLuint _leafTex, glm::vec3 _pos, glm::vec3 _volumePos)
{
	segCount = 0;
	growthFactor = 0;
	highestSegId = 0;
	growCount = 0;
	barkTex = _barkTex;
	leafTex = _leafTex;
	this->setPosition(_pos);
	volumePos = _volumePos;
	createLeaves();
	init();
	srand(time(NULL));
}

Plant::~Plant()
{
	for (int i = 0; i < limbs.size(); i++)
	{
		delete limbs.at(i);
	}
	for (int i = 0; i < leaves.size(); i++)
	{
		delete leaves.at(i);
	}
	for (int i = 0; i < particles.size(); i++)
	{
		delete particles.at(i);
	}
}


void Plant::update(Camera _camera, float _deltaTime)
{
	for (int i = 0; i < limbs.size(); i++)
	{
		limbs.at(i)->update(_camera, growthFactor, this->getTranformMatrix());
	}


	if (growthFactor <= highestSegId + 10)
	{
		growthFactor += (1 * _deltaTime);	

		for (int i = 0; i < particles.size(); i++)
		{
			if (particles.at(i)->getId() < growthFactor)
			{
				particles.at(i)->update(_deltaTime, _camera);
				particles.at(i)->render(_camera, this->getTranformMatrix());
			}
		}
	}
	else
	{
		for (int i = 0; i < particles.size(); i++)
		{
			particles.at(i)->update(_deltaTime, _camera);
			particles.at(i)->render(_camera, this->getTranformMatrix());
		}
	}


}

void Plant::init()
{	

	Segment* seg = new Segment();

	Limb* limb = new Limb(seg, 0, barkTex);
	limbs.push_back(limb);

	seg->position = limb->getPosition();
	seg->direction = limb->getUp();
	seg->origionalDir = seg->direction;
	segCount++;


	Segment* current = seg;

	bool found = false;
	int counter = 0;

	while (!found)
	{
		
		for (int i = leaves.size() - 1; i >= 0; i--)
		{
			float d = glm::distance(current->position, leaves.at(i)->position);
			if (d < MAX_DISTANCE)
			{
				found = true;
			}
		}

		if (!found)
		{
			Segment* newSeg = new Segment();
			
			newSeg->position = current->position + current->direction;
			newSeg->direction = current->direction;
			newSeg->origionalDir = current->direction;
			newSeg->parent = current;
			newSeg->id = newSeg->parent->id + 1;

			if (newSeg->id > highestSegId)
				highestSegId = newSeg->id;

			segCount++;

			limb->addSegment(newSeg);

			current->hasChild = true;
			current = newSeg;
		}
	}
	
	cout << segCount << endl;
	
	grow();
}

void Plant::grow()
{
	growCount++;
	//cout << growCount << endl;
	for (int i = leaves.size() - 1; i >= 0; i--)
	{
		
		Leaf* leaf = leaves.at(i);

		Segment* closestSeg = nullptr;

		float record = 1000000;

		for (int j = limbs.size() - 1; j >= 0; j--)
		{
			for (int k = limbs.at(j)->getSegments().size() - 1; k >= 0; k--)
			{
				Segment* seg = limbs.at(j)->getSegments().at(k);

				float d = glm::distance(leaf->position, seg->position);

				if (d < MIN_DISTANCE)
				{
					leaf->reached = true;
					closestSeg = nullptr;
					break;
				}
				else if (d > MAX_DISTANCE){
				}
				else if (closestSeg == nullptr || d < record)
				{
					closestSeg = seg;
					record = d;
				}
		
			}
		}

		if (closestSeg != nullptr)
		{
			glm::vec3 newDir = glm::normalize(leaf->position - closestSeg->position);

			closestSeg->direction += newDir;
			closestSeg->count += 1;
		}
	}

	for (int i = leaves.size() - 1; i >= 0; i--)
	{
		if (leaves.at(i)->reached == true)
		{
			Leaf* leaf = leaves.at(i);
			leaves.erase(leaves.begin() + i);
			delete leaf;
		}
	}

	bool found = false;

	for (int j = limbs.size() - 1; j >= 0; j--)
	{

		for (int k = limbs.at(j)->getSegments().size() - 1; k >= 0; k--)
		{
			Limb* limb = limbs.at(j);
			Segment* seg = limbs.at(j)->getSegments().at(k);
			
			if (seg->count > 0)
			{
				found = true;
				seg->direction /= seg->count;
				
				if (seg->hasChild)
				{
					Segment* newSeg = new Segment();
					
					Limb* newLimb = new Limb(newSeg, k+j, barkTex);


					newSeg->position = seg->position + seg->direction;
					newSeg->direction = seg->direction;
					newSeg->origionalDir = seg->direction;
					newSeg->parent = seg;
					newSeg->id = newSeg->parent->id + 1;

					if (newSeg->id > highestSegId)
						highestSegId = newSeg->id;
					
					limbs.push_back(newLimb);
					segCount++;
				}
				else
				{
					Segment* newSeg = new Segment();

					newSeg->position = seg->position + seg->direction;
					newSeg->direction = seg->direction;
					newSeg->origionalDir = seg->direction;
					newSeg->parent = seg;
					newSeg->id = newSeg->parent->id + 1;

					if (newSeg->id > highestSegId)
						highestSegId = newSeg->id;

					seg->hasChild = true;
					limb->addSegment(newSeg);
					segCount++;
				}
				seg->count = 0;
				seg->direction = seg->origionalDir;
			}
		}
	}

	if (found && growCount < 40)
	{
		grow();
	}
	else
	{
		for (int i = limbs.size() - 1; i >= 0; i--)
		{

			for (int j = limbs.at(i)->getSegments().size() - 1; j >= 0; j--)
			{
				Segment* seg = limbs.at(i)->getSegments().at(j);
			
			
				seg->size += 1;
				if (seg->parent != nullptr)
				{
					seg->parent->size += seg->size;
				}

				if (seg->size < 8)
				{
					ParticleEmitter *emitter = new ParticleEmitter(1, seg->position, seg->parent->position, leafTex, seg->id);
					particles.push_back(emitter);
				}
			
			}

			limbs.at(i)->createMesh(segCount);
		}
	}

}

void Plant::createLeaves()
{
	int count = 50;
	int radius = 5;

	createSphereVolume(volumePos + glm::vec3(0,  radius, 0), radius, count);
	createSphereVolume(volumePos + glm::vec3( radius, 0, 0), radius, count);
	createSphereVolume(volumePos + glm::vec3(-radius, 0, 0), radius, count);
	createSphereVolume(volumePos + glm::vec3(0, 0,  radius), radius, count);
	createSphereVolume(volumePos + glm::vec3(0, 0, -radius), radius, count);

}

void Plant::createSphereVolume(glm::vec3 _position, float _radius, int _count)
{
	float xPos, yPos, zPos;
	int range = _radius;


	for (int i = 0; i < _count; ++i)
	{
		xPos = (rand() % ((range * 2) + 1) + (-range));
		yPos = (rand() % ((range * 2) + 1) + (-range));
		zPos = (rand() % ((range * 2) + 1) + (-range));

		Leaf* leaf = new Leaf();
		leaf->position = glm::vec3(xPos, yPos, zPos) + _position;

		leaves.push_back(leaf);
	}
}

