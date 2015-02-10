#pragma once

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag

// Other libraries
#include <iostream>
#include <vector>

/* Used to access elements of a mesh */
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(aiMesh* m);
	int GetNumVertices();
	int GetNumFaces();
	// returns Mesh for model
	aiMesh* GetAllModelData();
	// return pointer to vertexData
	float* GetVertexData();
private:
	int numVertices;
	int numFaces;
	aiMesh* modelData;
	std::vector<float> vertexData;
	// pulls data from aiMesh's stucture and stores it in a lovely vector for us
	void StoreVertexData();
};