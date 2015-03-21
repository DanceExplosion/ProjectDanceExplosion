#pragma once

//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag

//GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Other libraries
#include <iostream>
#include <fstream>
#include <vector>

// Node's contain the data and function needed to represent the skeletons of a model
class Node
{
public:
	// Empty constructor
	Node();	
	// Normal constructor
	Node(const aiScene* mainScene);
	// Simple get functions
	float* GetBoneMatrix();
	int GetNumBones();

	// Function to print the node hierarchy
	void PreOrderTraversal();
	
	// Function to build the matrix list
	std::vector<float> StoreBones();
private:
	
	
	// Recursive print functions
	void PreOrder(aiNode* node);
	void PreOrder(aiNode* node, int count);

	// Find a node by name
	aiMatrix4x4 SearchTree(aiNode* node, aiString name);

	// The model scene
	const aiScene* mainScene;

	// The total number of bones
	int numBones;

	// The root node
	aiNode* root;

	// The array that holds the matrix data
	std::vector<float> boneMatricies;
	//std::vector<aiMatrix4x4> boneMatricies;
	//std::vector<glm::mat4> boneMatricies;
};