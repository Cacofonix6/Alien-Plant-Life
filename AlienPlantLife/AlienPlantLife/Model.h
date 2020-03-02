#ifndef MODEL_H
#define MODEL_H

#include "PlantMesh.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
	////// Variables //////
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh*> meshes;
	string directory;
	bool gammaCorrection;

	////// Constructor //////
	Model() {}
	Model(char *path);
	~Model();

	////// Functions //////
	void draw(Shader shader);
	void loadModel(string path);

private:
	
	////// Functions //////
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
};
#endif
