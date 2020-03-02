#include "Object.h"

Object::Object(const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath) : shader(_vertexPath, _fragmentPath, _geometryPath){}
Object::Object(Model _model, const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath) : shader(_vertexPath, _fragmentPath, _geometryPath)
{
	model = _model;
}
Object::Object(const char* _modelPath, const GLchar* _vertexPath, const GLchar* _fragmentPath, const char* _geometryPath) : shader(_vertexPath, _fragmentPath, _geometryPath)
{
	model.loadModel(_modelPath);
}

void Object::draw(Camera _camera)
{
	Shader normalShader("resources/Shaders/NormalViewer.vert", "resources/Shaders/NormalViewer.frag", "resources/Shaders/NormalViewer.geom");
	shader.use();
	shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("lightDir", -0.2f, -1.0f, -0.3f);
	shader.setVec3("viewPos", _camera.Position);

	glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom), (float)Window::WIDTH / (float)Window::HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = _camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glm::mat4 modelTransform;
	modelTransform = glm::translate(modelTransform, getPosition());
	modelTransform = glm::scale(modelTransform, getScale());
	shader.setMat4("model", getTranformMatrix());
	glm::mat4 world = projection * view * getTranformMatrix();

	model.draw(shader);

	normalShader.use();
	normalShader.setMat4("world", world);

	model.draw(normalShader);


}
void Object::loadModel(Model _model)
{
	model = _model;
}
void Object::loadModel(const char* _path)
{
	model.loadModel(_path);
}

void Object::addMesh(Mesh* _mesh)
{
	model.meshes.push_back(_mesh);
}
