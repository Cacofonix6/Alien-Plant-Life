#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace std;

class Transform
{
public:
	Transform();
	~Transform();

	void setPosition( glm::vec3 _value) { position = _value; }
	void setPosition( double _x,  double _y,  double _z) { position = glm::vec3(_x, _y, _z); }

	void setScale( glm::vec3 _value) { scale = _value; }
	void setScale( double _x,  double _y,  double _z) { scale = glm::vec3(_x, _y, _z); }
	void setScale( double _value) { scale *= _value; }
	
	glm::vec3 getPosition() { return position; }
	glm::vec3 getScale() { return scale; }
	glm::quat getOrientation() { return orientation; }
	glm::vec3 getUp() { return glm::normalize(orientation * glm::vec3(0, 1, 0)); }
	glm::vec3 getForward() { return glm::normalize(orientation * glm::vec3(0, 0, -1)); }
	glm::vec3 getRight() { return glm::normalize(orientation * glm::vec3(1, 0, 0)); }

	void setOrientation(double _xAngle, double _yAngle, double _zAngle);
	void setOrientation(glm::quat _orientation);

	static glm::quat rotationBetween(glm::vec3 _start, glm::vec3 _dest);
	static glm::quat rotateTowards(glm::quat _original, glm::quat _destination, float _maxAngle);
	static glm::vec3 rotationAxisBetween(glm::vec3 _start, glm::vec3 _dest);


	void rotateForwardTowards(glm::vec3 _destination, float _maxAngle);
	void rotateUpTowards(glm::vec3 _destination, float _maxAngle);
	void rotateRightTowards(glm::vec3 _destination, float _maxAngle);

	void upLookAt(glm::vec3 _target);
	void lookAt(glm::vec3 _target);


	glm::mat4 getTranformMatrix();

	void rotate(double _xRot, double _yRot, double _zRot);
	void rotate(glm::vec3 _rotAxis, double _rotAngle);
	void rotate(glm::quat _rot);

private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	//glm::vec3 up;
	//glm::vec3 forward;
	//glm::vec3 right;

};

#endif // !TRANSFORM_H
