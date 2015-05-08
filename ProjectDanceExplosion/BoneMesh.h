#pragma once

//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag
#include <assimp/cimport.h>

// DevIL libraries
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

//GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Other libraries
#include <iostream>
#include <fstream>
#include <vector>

/* Used to store elements of a mesh */
class BoneMesh
{
public:
	// constructors & destructors
	BoneMesh();
	BoneMesh(aiMesh* m, int boneIndex);
	~BoneMesh();

	void StoreBoneSpaceVertices();

	// Getters
	float* GetVertexData();
	aiVector3D GetVertexAt(int index);

private:
	aiMesh* modelData;	// contains mesh of entire model - needs deleted after creation
	aiBone* meshBone;	// bone to which data refers

	std::vector<aiVector3D> vertexData;
};