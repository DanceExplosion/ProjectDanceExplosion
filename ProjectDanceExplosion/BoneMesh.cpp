#include "BoneMesh.h"

BoneMesh::BoneMesh() {}
BoneMesh::~BoneMesh() {}

BoneMesh::BoneMesh(aiMesh* m, int boneIndex)
{
	modelData = m;
	meshBone = modelData->mBones[boneIndex];

	m = NULL;
}

void BoneMesh::StoreBoneSpaceVertices(aiMatrix4x4 inverseBind)
{
	for(unsigned int i = 0; i < meshBone->mNumWeights; i++)
	{
		int vertexIndex = meshBone->mWeights[i].mVertexId;
		aiVector3D currentVertex = modelData->mVertices[vertexIndex];

		// Store vertex data
		vertexData.push_back(currentVertex);
	}
}

aiVector3D BoneMesh::GetVertexAt(int index)
{
	return vertexData.at(index);
}

float* BoneMesh::GetVertexData()
{
	return &vertexData.at(0).x;
}
