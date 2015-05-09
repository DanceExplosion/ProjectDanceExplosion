//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag

// Inner Project Classes
#include "Skinning.h"
#include "ModelAnimation.h"
#include "Node.h"

class ModelData
{
public:
	// Importer loads the model and creates a scene object
	Assimp::Importer importer;

	// Scene, holds all of the model data
	const aiScene* scene;

	// Controls and Updates the nodes in the scene acording to animation data
	AnimationController animCont;

	// List of meshes used for skinning
	std::vector<Skinning> skinList;

	// List of animation times for multi-animation models
	std::vector<glm::vec2> animationTimes;

	// Node class collects the node data from the model's skeleton and prepares it for rendering
	Node modelNodes;

	// Load a model from the given file+directory
	void LoadModelData(std::string ModelFile);

	void Update();
};