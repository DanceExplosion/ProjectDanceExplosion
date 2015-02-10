#include "Mesh.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

Mesh::Mesh(aiMesh* m)
{
	numVertices = m->mNumFaces * 3; // more accurate than m->mNumVerticies
	numFaces = m->mNumFaces;
	modelData = m;
	// will evert model have verticies? or should we check for them?
	StoreVertexData();
	// can check for other data and store accordingly
}

void Mesh::StoreVertexData()
{
	// for each face in mesh
	for (unsigned int i = 0; i < numFaces; i++)
	{
		const aiFace& currentFace = modelData->mFaces[i];

		// each face has 3 vertices
		for (unsigned int j = 0; j < 3; j++)
		{
			aiVector3D pos = modelData->mVertices[currentFace.mIndices[j]];
			// pull vertex.x, y & z to a more convenient location
			// vertices require scaling to fit in window
			vertexData.push_back(pos.x);// * 0.08f);
			vertexData.push_back(pos.y);// * 0.08f);
			vertexData.push_back(pos.z);// * 0.08f);
		}
	}
}

aiMesh* Mesh::GetAllModelData()
{
	return modelData;
}

int Mesh::GetNumFaces()
{
	return numFaces;
}

// as this is calculated when Mesh is created this is accurate for this mesh
int Mesh::GetNumVertices()
{
	return numVertices;
}

float* Mesh::GetVertexData()
{
	// reference first element of vector
	return &vertexData.at(0);
}