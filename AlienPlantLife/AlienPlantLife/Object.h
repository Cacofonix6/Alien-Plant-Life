#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"
#include "Camera.h"
#include "Window.h"

class Object : public Transform
{
public:
	Model model;

	Object(const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath = nullptr);
	Object(Model _model, const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath);
	Object(const char* _modelPath, const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath);

	void draw(Camera _camera);
	void loadModel(Model _model);
	void loadModel(const char* _path);
	void loadCube();

	void addMesh(Mesh* _mesh);

protected:
	Shader shader;

};

#endif // !OBJECT_H
