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

class Node
{
public:
	Node();
	Node(const aiScene* mainScene);
	float* GetBoneMatrix();
	int GetNumBones();

	void StoreBones();

	void PreOrderTraversal();
	void PreOrder(aiNode* node);
	void PreOrder(aiNode* node, int count);
	aiMatrix4x4 SearchTree(aiNode* node, aiString name);
	aiNode* Node::SearchTreeForNode(aiNode* node, aiString name);
	
	void SkinBone(int meshIndex, int boneIndex);
	float* GetKnee();
	int Node::GetKneeSize();

private:
	const aiScene* mainScene;
	int numBones;
	aiNode* root;
	std::vector<float> boneMatricies;

	std::vector<float> knee;
	//std::vector<aiMatrix4x4> boneMatricies;
	//std::vector<glm::mat4> boneMatricies;
};