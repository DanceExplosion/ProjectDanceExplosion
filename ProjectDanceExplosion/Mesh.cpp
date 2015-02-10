#include "Mesh.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

Mesh::Mesh(aiMesh* m)
{
	numVertices = m->mNumVertices;
	numFaces = m->mNumFaces;
	modelData = m;
}

aiMesh* Mesh::GetAllModelData()
{
	return modelData;
}

int Mesh::GetNumFaces()
{
	return numFaces;
}

int Mesh::GetNumFaces(aiMesh* m)
{
	return m->mNumFaces;
}

int Mesh::GetNumVertices()
{
	return numVertices;
}

int Mesh::GetNumVertices(aiMesh* m)
{
	return m->mNumVertices;
}