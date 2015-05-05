#pragma once

//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag
#include <assimp/cimport.h>

//GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Other libraries
#include <iostream>
#include <fstream>
#include <vector>

// Project headers
#include "BoneMesh.h"

// Node's contain the data and function needed to represent the skeletons of a model
class Skinning
{
public:
	// Empty constructor
	Skinning();	
	// Normal constructor
	Skinning(const aiScene* mainScene);
	
	
	// Store all vertices in corresponding bone space
	void StoreAllVertices();

	/* Using BoneMesh */

	// Creates an array of Mesh objects
	void StoreBoneMeshes(int meshIndex);
	// Move vertices to world space using modelMeshes vector
	void MoveMeshToWorldSpace(int meshIndex, int boneIndex);

	void StoreFinalVertexData(int meshIndex);
	/* End Using BoneMesh*/
	
	// Fill vector Changed with FALSE
	void ResetChanged();

	int GetNumVertices();
	GLuint GetTextureRef();
	GLuint GetNormalMapRef();
	//
	float* GetWorldSpaceVertices();
	float* GetTextureCoordinates();

private:	
	aiMatrix4x4 SearchTree(aiNode* node, aiString name);

	void StoreNormalMapData(aiMaterial* mat);
	void StoreTextureData(aiMaterial* mat);

	void StoreTextureCoordData(aiMesh* modelData, aiFace currentFace, int index);

	GLuint textureRef;		// diffuse texture reference for bone
	GLuint normalMapRef;	// normal texture reference for bone

	const aiScene* mainScene;
	aiNode* root;

	std::vector<BoneMesh> modelMeshes;

	// vertices in bone space
	std::vector<glm::vec3> boneSpaceVertices;
	// vertices in world space
	std::vector<glm::vec3> worldSpaceVertices;

	// vertices used to draw model
	std::vector<glm::vec3> finalVertexData;
	// texture co-ordinates for model
	std::vector<glm::vec2> textureCoordData;
	// normals for model
	std::vector<glm::vec3> normalData;

	// has vertex already been move to worldSpace?
	std::vector<bool> changed; 

};