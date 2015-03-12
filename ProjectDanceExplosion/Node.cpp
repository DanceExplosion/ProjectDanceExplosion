#include "Node.h"

// Empty Initializer
Node::Node() {}

// Basic Constructor
Node::Node(const aiScene* mS)
{
	// Store the pointer to the scene
	mainScene = mS;

	// Obtain the root node of the skeleton
	root = mainScene->mRootNode;

	// Set the total number of bones to 0
	numBones = 0;

	// Call the StoreBones method
	StoreBones();

	// Print out the hierarchy
	// PreOrderTraversal();
}

// Gather the data needed for the skeleton
void Node::StoreBones()
{
	// Loop through each mesh in the scene
	for(unsigned int m = 0; m < mainScene->mNumMeshes; m++)
	{
		// Store the mesh's data
		aiMesh* modelData = mainScene->mMeshes[m];

		// Add the mesh's bone count to the total number of bones
		numBones += modelData->mNumBones;

		// Loop through all the mesh's bones
		for(unsigned int i = 0; i < modelData->mNumBones; i++)
		{
			// Store it's location for easy access
			aiBone* bone1 = modelData->mBones[i];

			// Same again, but for it's name
			aiString name = bone1->mName;

			// Get the number of vertices affected by the bone
			unsigned int numWeights = bone1->mNumWeights;

			// Inverse and Transpose teh bone's data
			aiMatrix4x4 bonePlace = bone1->mOffsetMatrix.Inverse().Transpose();// * SearchTree(root, name);

			// *old* Convert to glm's mat4 format
			/*glm::mat4 boneFinal = glm::mat4(bonePlace.a1, bonePlace.a2, bonePlace.a3, bonePlace.a4,
												bonePlace.b1, bonePlace.b2, bonePlace.b3, bonePlace.b4,
												bonePlace.c1, bonePlace.c2, bonePlace.c3, bonePlace.c4,
												bonePlace.d1, bonePlace.d2, bonePlace.d3, bonePlace.d4);
												*/
			
			// *old* Print out the matrix data
			/*std::cout << "Bone[" << i << "]" << std::endl;
			std::cout << "	Bone Name: " << name.data << std::endl;
			std::cout << "\tTransformation: " << bonePlace.a1 << " " << bonePlace.a2 << " " << bonePlace.a3 << " " << bonePlace.a4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.b1 << " " << bonePlace.b2 << " " << bonePlace.b3 << " " << bonePlace.b4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.c1 << " " << bonePlace.c2 << " " << bonePlace.c3 << " " << bonePlace.c4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.d1 << " " << bonePlace.d2 << " " << bonePlace.d3 << " " << bonePlace.d4 << std::endl;*/

			// *old push the matrices into the list
			//boneMatricies.push_back(bonePlace);
			//boneMatricies.push_back(boneFinal);

			// Push the matrix values into the float list for passing into the shader

			// Row 1
			boneMatricies.push_back(bonePlace.a1);
			boneMatricies.push_back(bonePlace.a2);
			boneMatricies.push_back(bonePlace.a3);
			boneMatricies.push_back(bonePlace.a4);
			
			// Row 2
			boneMatricies.push_back(bonePlace.b1);
			boneMatricies.push_back(bonePlace.b2);
			boneMatricies.push_back(bonePlace.b3);
			boneMatricies.push_back(bonePlace.b4);

			// Row 3
			boneMatricies.push_back(bonePlace.c1);
			boneMatricies.push_back(bonePlace.c2);
			boneMatricies.push_back(bonePlace.c3);
			boneMatricies.push_back(bonePlace.c4);

			// Row 4
			boneMatricies.push_back(bonePlace.d1);
			boneMatricies.push_back(bonePlace.d2);
			boneMatricies.push_back(bonePlace.d3);
			boneMatricies.push_back(bonePlace.d4);

		}
	}

	/*aiBone* bone1 = modelData->mBones[4];
	aiString name = bone1->mName;
	unsigned int numWeights = bone1->mNumWeights;
	std::cout << "Bone[4]" << std::endl;
	std::cout << "	Bone Name: " << name.data << std::endl;
	aiMatrix4x4 bonePlace = SearchTree(root, name); //bone1->mOffsetMatrix;
	std::cout << "\tTransformation: " << bonePlace.a1 << " " << bonePlace.a2 << " " << bonePlace.a3 << " " << bonePlace.a4 << std::endl;
	std::cout << "\t \t \t" << bonePlace.b1 << " " << bonePlace.b2 << " " << bonePlace.b3 << " " << bonePlace.b4 << std::endl;
	std::cout << "\t \t \t" << bonePlace.c1 << " " << bonePlace.c2 << " " << bonePlace.c3 << " " << bonePlace.c4 << std::endl;
	std::cout << "\t \t \t" << bonePlace.d1 << " " << bonePlace.d2 << " " << bonePlace.d3 << " " << bonePlace.d4 << std::endl;*/

	int num = 9;
}

//Initial method for printing the Hierarchy
void Node::PreOrderTraversal()
{
	int count = 0;
	PreOrder(root, count);
}

// Recursive method for printing the hierarchy
void Node::PreOrder(aiNode* node, int count)
{
	count++;
	// If the parent is null, then this node is the root
	if(node->mParent == NULL)
	{
		// Print out it's name
		std::cout << "Root: " << node->mName.data << std::endl;

		// *old* Print matrix data
		/*std::cout << "Transformation: " << node->mTransformation.a1 << " " << node->mTransformation.a2 << " " << node->mTransformation.a3 << " " << node->mTransformation.a4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.b1 << " " << node->mTransformation.b2 << " " << node->mTransformation.b3 << " " << node->mTransformation.b4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.c1 << " " << node->mTransformation.c2 << " " << node->mTransformation.c3 << " " << node->mTransformation.c4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.d1 << " " << node->mTransformation.d2 << " " << node->mTransformation.d3 << " " << node->mTransformation.d4 << std::endl;*/
	}
	// If the node isn't the root
	else
	{
		// Print out it's parent, and it's 'depth' in the hierarchy
		std::cout << "Parent: " << node->mParent->mName.data << " Level: " << count << std::endl;

		// Print out it's name
		std::cout << "	Node: " << node->mName.data << std::endl;

		// *old* Print out the affected meshes
		/*if(node->mMeshes != 0)
		{
			std::cout << "	Mesh detected" << std::endl;
			std::cout << "	NumMeshes: " << node->mMeshes << std::endl;
		}*/

		// Print out it's transformation data
		std::cout << "\tTransformation: " << node->mTransformation.a1 << " " << node->mTransformation.a2 << " " << node->mTransformation.a3 << " " << node->mTransformation.a4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.b1 << " " << node->mTransformation.b2 << " " << node->mTransformation.b3 << " " << node->mTransformation.b4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.c1 << " " << node->mTransformation.c2 << " " << node->mTransformation.c3 << " " << node->mTransformation.c4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.d1 << " " << node->mTransformation.d2 << " " << node->mTransformation.d3 << " " << node->mTransformation.d4 << std::endl;
	}
	
	// If the node has children
	if(node->mNumChildren > 0)
	{
		// Recursively call this function for all it's children
		for(unsigned int i = 0; i < node->mNumChildren; i++)
			PreOrder(node->mChildren[i], count);
	}
}

// Recursive method for finding the node with a requested name
aiMatrix4x4 Node::SearchTree(aiNode* node, aiString name)
{
	// Store the name for easy access
	aiString currentName = node->mName;

	// Matrix to be returned
	aiMatrix4x4 mat;

	// Corresponding node found
	if (name == currentName)
	{
		// *old* Report that the node was found
		//std::cout << name.data << " was found" << std::endl;

		// Store the transformation
		aiMatrix4x4 finalTransform = node->mTransformation;

		// Get the total transformation of the node
		while(node->mParent != NULL)
		{
			node = node->mParent;
			finalTransform = finalTransform * node->mTransformation;
		}

		//Return the total matrix
		return finalTransform.Transpose();
	}
	// If the name doesn't match
	else
	{
		// Loop through all of the current node's children
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			// Get the return value of the recursive search
			aiMatrix4x4 mat = SearchTree(node->mChildren[i], name);

			// If it has returned a value that isn't NULL, return it
			if(mat.a1 != NULL)
				return mat;
		}
	}

	// If nothing has been found, then this branch of the skeleton is a dead end, return NULL for the first value to represent this
	return aiMatrix4x4(	NULL,0,0,0,
						0,0,0,0,
						0,0,0,0,
						0,0,0,0);
}

// *old* Alternate Search Tree Method
/*aiMatrix4x4 Node::SearchTree(aiNode* node, aiString name)
{
	aiString currentName = node->mName;

	//corresponding node found
	if (name == currentName)
	{
		std::cout << name.data << " was found" << std::endl;
		aiMatrix4x4 finalTransform = node->mTransformation;
		while(node->mParent != NULL)
		{
			node = node->mParent;
			finalTransform = finalTransform * node->mTransformation;
		}
		return finalTransform;
	}
	else
	{
		for(int i = 0; i < node->mNumChildren; i++)
			return SearchTree(node->mChildren[i], name);
	}
}*/


// Simple return function for the bone matrix
float* Node::GetBoneMatrix()
{
	return &boneMatricies.at(0);//.a1;
	//return glm::value_ptr(boneMatricies.at(0));
}

// Simple return for the number of bones
int Node::GetNumBones()
{
	return numBones;
}