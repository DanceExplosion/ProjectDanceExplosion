#include "ModelAnimation.h"

	// Animation controller collects data from assimp and updated the node tree with the data from a selected animation

	// Simple update function
void AnimationController::Update(float delta){
	// Advance the animation's time
	timePassed = timePassed + (delta * 0.016*animationSpeed);
	//timePassed = timePassed + (delta * 0.001);
	//timePassed = timePassed + (delta * 2.0);

	// Loop the animation
	if(timePassed >= endTime){
		timePassed = startTime;
	}

	// Rotation Matrix
	aiMatrix3x3 R = aiMatrix3x3();

	// Loop through every channel in the current animation
	for(int i = 0; i<currentAnimation->mNumChannels;i++){

		// Set up for the node's position
		#pragma region Position Vector
		// Declare a variable that will store the translation value
		aiVector3D temp;

		// Start from the second KeyFrame (the current position and the one behind it are used)
		int currentKeyPosition = 1;

		// If the animation has several translation keys, then it needs to be calculated
		if(1 < currentAnimation->mChannels[i]->mNumPositionKeys){

			// Loop until the current keyframe is found
			while(true){
				// If the time passes the time in the currentKey's position move onto the next key position
				// Reset if it has reached the end
				if(timePassed >= currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition].mTime){
					if(currentKeyPosition < currentAnimation->mChannels[i]->mNumPositionKeys)
						currentKeyPosition++;
					else
						break;
				}else
					break;
			}

			// Get the time from the first frame
			double startTime = currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition-1].mTime;

			// Get the time from the second frame
			double endTime = currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition].mTime;

			// Get the value of the first frame
			aiVector3D start = currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition-1].mValue;

			// Get the value of the second frame
			aiVector3D end =  currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition].mValue;
			
			// Get the percent of how far the node is through the transition
			double percent = (timePassed - startTime)/(endTime-startTime);

			// Interpolate using the above data
			temp = Interpolate(start, end, percent);

			// If there is only one translation frame, then use it and nothing else
		}else{
			temp = currentAnimation->mChannels[i]->mPositionKeys[0].mValue;
		}
		#pragma endregion

		// Set up for the node's rotation
		#pragma region Rotation Quarternion

		// Start from the second KeyFrame (the current rotation and the one behind it are used)
		int currentKeyRotation = 1;

		// Loop until the current keyframe is found
		while(true){
			// If the time passes the time in the currentKey's position move onto the next key position
			// Reset if it has reached the end
			if(timePassed >= currentAnimation->mChannels[i]->mRotationKeys[currentKeyRotation].mTime){
				if(currentKeyRotation < currentAnimation->mChannels[i]->mNumRotationKeys)
					currentKeyRotation++;
				else
					currentKeyRotation = 1;
			}else
				break;
		}
		
		// Get the time from the first frame
		double startTime = currentAnimation->mChannels[i]->mRotationKeys[currentKeyRotation-1].mTime;

		// Get the time from the second frame
		double endTime = currentAnimation->mChannels[i]->mRotationKeys[currentKeyRotation].mTime;

		// Get the value of the first frame
		aiQuaternion rStart = currentAnimation->mChannels[i]->mRotationKeys[currentKeyRotation-1].mValue;

		// Get the value of the second frame
		aiQuaternion rEnd =  currentAnimation->mChannels[i]->mRotationKeys[currentKeyRotation].mValue;
		
		// Get the percent of how far the node is through the transition
		double percent = (timePassed - startTime)/(endTime-startTime);

		// Create a Quaternion
		aiQuaternion rTemp = aiQuaternion();

		// Use Assimp's Interpolate function to calculate the resulting quaternion
		aiQuaternion::Interpolate(rTemp,rStart,rEnd,percent);

		// Get the 3x3 matric from the quaternion
		R = rTemp.GetMatrix();
		#pragma endregion

		// *old* Scaling code, scaling is not yet compatable
		/*
		#pragma region Scale Vector
		// If the time passes the time in the currentKey's position move onto the next key position
		// Reset if it has reached the end
		if(timePassed >= currentAnimation->mChannels[i]->mPositionKeys[currentKeyPosition].mTime){
			if(currentKeyScale < currentAnimation->mChannels[i]->mNumScalingKeys)
				currentKeyScale++;
			else
				currentKeyScale = 0;
		}
		startTime = currentAnimation->mChannels[i]->mScalingKeys[currentKeyScale-1].mTime;
		endTime = currentAnimation->mChannels[i]->mScalingKeys[currentKeyScale].mTime;

		start = currentAnimation->mChannels[i]->mScalingKeys[currentKeyScale-1].mValue;
		end =  currentAnimation->mChannels[i]->mScalingKeys[currentKeyScale].mValue;
		
		percent = (timePassed - startTime)/(endTime-startTime);
		temp = Interpolate(start, end, percent);
		S.Translation(temp,T);
		#pragma endregion
		*/

		// Get a 4x4 matrix for the total transformation, and pass in the Quaternion
		aiMatrix4x4 total = aiMatrix4x4(R);

		// Pass in the translation data
		total.a4 = temp.x;
		total.b4 = temp.y;
		total.c4 = temp.z;

		// Pass the data into the channel's affected node
		SearchTree(rootNode,currentAnimation->mChannels[i]->mNodeName,total);
	}
}

	// Interpolate find the transition between two nodes that is 'percentage' of the way through
aiVector3D AnimationController::Interpolate(aiVector3D start, aiVector3D end, double percentage){
	aiVector3D distance = end-start;
	distance*=percentage;
	return start+distance;
}

// Recursive method for finding the node with a requested name
void AnimationController::SearchTree(aiNode* node, aiString name, aiMatrix4x4 transform)
{
	// Store the name for easy access
	aiString currentName = node->mName;

	// Matrix to be returned
	aiMatrix4x4 mat;

	// Corresponding node found
	if (name == currentName)
	{
		/*
		aiMatrix4x4 temp = aiMatrix4x4(transform.a1,transform.a2,transform.a3,node->mTransformation.a4,
										transform.b1,transform.b2,transform.b3,node->mTransformation.b4,
										transform.c1,transform.c2,transform.c3,node->mTransformation.c4,
										transform.d1,transform.d2,transform.d3,transform.d4);
										*/
		// Set the matrix of the node found
		node->mTransformation = transform;
	}
	// If the name doesn't match
	else
	{
		// Loop through all of the current node's children
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			// Get the return value of the recursive search
			SearchTree(node->mChildren[i], name, transform);
		}
	}
}
