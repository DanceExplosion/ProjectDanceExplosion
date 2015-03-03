#pragma once

//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag

// DevIL libraries
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

// Other libraries
#include <iostream>
#include <vector>

/* Used to store elements of a mesh */
class Mesh
{
public:
	// constructors & destructors
	Mesh();
	Mesh(aiMesh* m, aiMaterial* mat);
	~Mesh();

	int GetNumVertices();
	int GetAINumVertices();
	int GetNumFaces();

	// return pointers to Vertex, Normal and TextureCoord data
	float* GetVertexData();
	float* GetNormalData();
	float* GetTextureCoords();
	// return textureRef
	GLuint GetTextureData();

private:
	aiMesh* modelData; // used to build object, then discarded

	int numVertices; // total number of vertices stored by mesh
	int aiNumVertices; // number of textured/coloured vertices
	int numFaces;
	GLuint textureRef;

	std::vector<float> vertexData;
	std::vector<float> normalData;
	std::vector<float> textureCoordData;

	// pulls data from aiMesh's stucture and stores it in a loovely vector for us
	void StoreVertexData();
	void StoreNormalData(aiFace currentFace, int index);
	void StoreTextureCoordData(aiFace currentFace, int index);
	void StoreTextureData(aiMaterial* mat);
};