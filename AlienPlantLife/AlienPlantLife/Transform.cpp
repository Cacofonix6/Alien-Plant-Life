#include "Transform.h"
#include <iostream>

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

Transform::~Transform() {}

void Transform::setOrientation(double _xAngle, double _yAngle, double _zAngle)
{
	glm::vec3 EulerAngles(glm::radians(_xAngle), glm::radians(_yAngle), glm::radians(_zAngle));

	orientation = glm::quat(EulerAngles);

}

void Transform::setOrientation(glm::quat _orientation)
{
	orientation = _orientation;
}

glm::quat Transform::rotationBetween(glm::vec3 _start, glm::vec3 _dest)
{
	_start = glm::normalize(_start);
	_dest = glm::normalize(_dest);

	float dotProd = glm::dot(_start, _dest);
	glm::vec3 rotAxis;

	// if vectors are in opposite directions
	if (dotProd < -1 + 0.001f)
	{
		rotAxis = glm::cross(glm::vec3(0, 0, 1), _start);
		if (glm::length2(rotAxis) < 0.01f)
		{
			rotAxis = glm::cross(glm::vec3(1, 0, 0), _start);
		}

		rotAxis = glm::normalize(rotAxis);

		return glm::angleAxis(glm::radians(180.0f), rotAxis);
	}

	rotAxis = glm::cross(_start, _dest);

	float s = glm::sqrt((1 + dotProd) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotAxis.x * invs,
		rotAxis.y * invs,
		rotAxis.z * invs
	);
}


glm::quat Transform::rotateTowards(glm::quat _original, glm::quat _destination, float _maxAngle)
{
	if (_maxAngle < 0.001f)
	{
		// No rotation alloweds
		return _original;
	}

	float dotProd = glm::dot(_original, _destination);

	if (dotProd > 0.9999f)
	{
		// they're already equal
		return _destination;
	}

	if (dotProd < 0)
	{
		//avoid taking the long way around
		_original = _original * -1.0f;
		dotProd *= -1.0f;
	}

	float angle = glm::acos(dotProd);

	if (angle < _maxAngle)
	{
		return _destination;
	}

	float fT = _maxAngle / angle;
	angle = _maxAngle;

	glm::quat res = (glm::sin((1.0f - fT) * angle) * _original + glm::sin(fT * angle) * _destination) / glm::sin(angle);
	res = glm::normalize(res);

	return res;
}

glm::vec3 Transform::rotationAxisBetween(glm::vec3 _start, glm::vec3 _dest)
{
	_start = glm::normalize(_start);
	_dest = glm::normalize(_dest);

	float dotProd = glm::dot(_start, _dest);
	glm::vec3 rotAxis;

	// if vectors are in opposite directions
	if (dotProd < -1 + 0.001f)
	{
		rotAxis = glm::cross(glm::vec3(0, 0, 1), _start);
		if (glm::length2(rotAxis) < 0.01f)
		{
			rotAxis = glm::cross(glm::vec3(1, 0, 0), _start);
		}

		rotAxis = glm::normalize(rotAxis);

		return rotAxis;
	}

	rotAxis = glm::cross(_start, _dest);

	return rotAxis;
}

void Transform::rotateForwardTowards(glm::vec3 _destination, float _maxAngle)
{
	glm::quat dest = rotationBetween(getForward(), _destination);

	rotate(rotateTowards(orientation, dest, _maxAngle));
}

void Transform::rotateUpTowards(glm::vec3 _destination, float _maxAngle)
{	
	glm::vec3 destDir = glm::normalize(_destination);
	
	glm::vec3 rotAxis = glm::normalize(rotationAxisBetween(getUp(), destDir));
	
	float angleBetween = glm::dot(getUp(), destDir);
	cout << angleBetween << endl;

	if (angleBetween > 0.999999f)
	{
		return;
	}

	angleBetween = glm::acos(angleBetween);
	angleBetween *= (180.0f / 3.14159265f);


	
	if (angleBetween > _maxAngle)
	{
		rotate(rotAxis, _maxAngle);
	}
	else 
	{
		rotate(rotAxis, angleBetween);
		//cout << getUp().x << ", " << getUp().y << ", " << getUp().z << endl;
	}

}

void Transform::rotateRightTowards(glm::vec3 _destination, float _maxAngle)
{
	glm::quat dest = rotationBetween(getRight(), _destination);

	setOrientation(rotateTowards(orientation, dest, _maxAngle));
}

void Transform::upLookAt(glm::vec3 _target)
{
	//glm::vec3 destDir = glm::normalize(_target);
	//
	//glm::vec3 rotAxis = glm::normalize(rotationAxisBetween(getUp(), destDir));
	//
	//float angleBetween = glm::dot(getUp(), destDir);
	//cout << angleBetween << endl;
	//
	//if (angleBetween > 0.999999f)
	//{
	//	return;
	//}
	//
	//angleBetween = glm::acos(angleBetween);
	//angleBetween *= (180.0f / 3.14159265f);
	//
	//
	//
	//if (angleBetween > _maxAngle)
	//{
	//	rotate(rotAxis, _maxAngle);
	//}
	//else
	//{
	//	rotate(rotAxis, angleBetween);
	//	cout << up.x << ", " << up.y << ", " << up.z << endl;
	//}

}

void Transform::lookAt(glm::vec3 _target)
{
	glm::vec3 destDir = glm::normalize(_target - position);
	
	glm::vec3 rotAxis = glm::normalize(rotationAxisBetween(getForward(), destDir));
	
	float angleBetween = glm::dot(getForward(), destDir);
	//cout << angleBetween << endl;
	
	if (angleBetween > 0.999999f)
	{
		return;
	}
	
	angleBetween = glm::acos(angleBetween);
	angleBetween *= (180.0f / 3.14159265f);
	
	
	rotate(rotAxis, angleBetween);
	//cout << up.x << ", " << up.y << ", " << up.z << endl;

}

glm::mat4 Transform::getTranformMatrix()
{
	glm::mat4 translate = glm::translate(glm::mat4(), getPosition());
	glm::mat4 rotate = glm::toMat4(orientation);
	glm::mat4 scale = glm::scale(glm::mat4(), getScale());

	return translate * rotate * scale;
}

void Transform::rotate(double _xRot, double _yRot, double _zRot)
{
	glm::vec3 EulerAngles(glm::radians(_xRot), glm::radians(_yRot), glm::radians(_zRot));
	
	glm::quat rot = glm::quat(EulerAngles);

	orientation *= rot;
}

void Transform::rotate(glm::vec3 _rotAxis, double _rotAngle)
{
	glm::quat rot = glm::angleAxis((float)glm::radians(_rotAngle), _rotAxis);

	orientation *= rot;
}

void Transform::rotate(glm::quat _rot)
{
	orientation = _rot;
}