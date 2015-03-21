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

// Controls a model's animation, only compatable with node-based animation, not vertex animation
class AnimationController{
	public:
	// List of all the model's animations
	aiAnimation** animationList;

	// The currently running animation
	aiAnimation* currentAnimation;

	// The current Key Frames of the animation (transitioning to)
	int currentKeyPosition,
		currentKeyRotation,
		currentKeyScale;
	
	// The target time for moving onto the next keyFrame
	double targetTimePos,
			targetTimeRot,
			targetTimeScale;
			

	// The elapsed time of the animation
	double timePassed;

	// Root node of hierarchy
	aiNode* rootNode;

	// Empty Constructor
	AnimationController(){}

	// Basic Constructor
	AnimationController(aiAnimation** aList, aiNode* root){
		animationList = aList;
		SetCurrentAnimation(0);
		rootNode = root;
	}

	// Change the animation for files with split animations
	void SetCurrentAnimation(int animationID){
		currentAnimation = animationList[animationID];
		timePassed = 0;
		currentKeyScale = 1;
	}

	// Update the animation
	void Update(float delta);

private:
	// Get the current position between the two vectors
	aiVector3D Interpolate(aiVector3D start, aiVector3D end, double percentage);

	// Find the node affected by the current channel and update it
	void SearchTree(aiNode* node, aiString name, aiMatrix4x4 transform);
};