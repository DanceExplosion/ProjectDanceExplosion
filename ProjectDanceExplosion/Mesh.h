#pragma once

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag

// Other libraries
#include <iostream>

/* Used to access elements of a mesh */
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(aiMesh* m);
	int GetNumVertices(aiMesh* m);
	int GetNumVertices();
	int GetNumFaces(aiMesh* m);
	int GetNumFaces();
	//returns Mesh for model
	aiMesh* GetAllModelData();
private:
	int numVertices;
	int numFaces;
	aiMesh* modelData;
};