#include "Node.h"

Node::Node() {}

Node::Node(const aiScene* mS)
{
	mainScene = mS;
	root = mainScene->mRootNode;
	numBones = 0;

	StoreBones();
}

void Node::StoreBones()
{
	for(unsigned int m = 0; m < mainScene->mNumMeshes; m++)
	{
		aiMesh* modelData = mainScene->mMeshes[m];
		numBones += modelData->mNumBones;

		for(unsigned int i = 0; i < modelData->mNumBones; i++)
		{
			aiBone* bone1 = modelData->mBones[i];
			aiString name = bone1->mName;
			//aiMatrix4x4 bonePlace = bone1->mOffsetMatrix.Inverse().Transpose();
			aiMatrix4x4 bonePlace = SearchTree(root,name);
			
			/*glm::mat4 boneFinal = glm::mat4(bonePlace.a1, bonePlace.a2, bonePlace.a3, bonePlace.a4,
												bonePlace.b1, bonePlace.b2, bonePlace.b3, bonePlace.b4,
												bonePlace.c1, bonePlace.c2, bonePlace.c3, bonePlace.c4,
												bonePlace.d1, bonePlace.d2, bonePlace.d3, bonePlace.d4);
												*/
			
			/*std::cout << "Bone[" << i << "]" << std::endl;
			std::cout << "	Bone Name: " << name.data << std::endl;
			std::cout << "\tTransformation: " << bonePlace.a1 << " " << bonePlace.a2 << " " << bonePlace.a3 << " " << bonePlace.a4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.b1 << " " << bonePlace.b2 << " " << bonePlace.b3 << " " << bonePlace.b4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.c1 << " " << bonePlace.c2 << " " << bonePlace.c3 << " " << bonePlace.c4 << std::endl;
			std::cout << "\t \t \t" << bonePlace.d1 << " " << bonePlace.d2 << " " << bonePlace.d3 << " " << bonePlace.d4 << std::endl;*/

			boneMatricies.push_back(bonePlace.a1);
			boneMatricies.push_back(bonePlace.a2);
			boneMatricies.push_back(bonePlace.a3);
			boneMatricies.push_back(bonePlace.a4);

			boneMatricies.push_back(bonePlace.b1);
			boneMatricies.push_back(bonePlace.b2);
			boneMatricies.push_back(bonePlace.b3);
			boneMatricies.push_back(bonePlace.b4);

			boneMatricies.push_back(bonePlace.c1);
			boneMatricies.push_back(bonePlace.c2);
			boneMatricies.push_back(bonePlace.c3);
			boneMatricies.push_back(bonePlace.c4);

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

void Node::SkinBone(int meshIndex, int boneIndex)
{
	std::vector<glm::vec3> boneVertices;
	std::vector<float> boneVertexWeights;

	aiMesh* mesh = mainScene->mMeshes[meshIndex];
	aiBone* bone = mesh->mBones[boneIndex];

	// taking aiNode as inverse bind
	aiMatrix4x4 inverseNodePose = SearchTree(root, bone->mName).Inverse();

	aiMatrix4x4 identity;
	aiIdentityMatrix4(&identity);

	// extracting vertices and vertex weights 
	for(unsigned int i = 0; i < bone->mNumWeights; i++)
	{
		unsigned int vertexIndex = bone->mWeights[i].mVertexId;
		float weight = bone->mWeights[i].mWeight;

		aiVector3D vTemp = mesh->mVertices[vertexIndex];
		// multiply by node inverse bind
		aiTransformVecByMatrix4(&vTemp, &inverseNodePose);
		glm::vec3 vertex = glm::vec3(vTemp.x, vTemp.y, vTemp.z);

		boneVertices.push_back(vertex);
		boneVertexWeights.push_back(weight);
	}

	// aiNode as currentPoseMatrix
	aiMatrix4x4 currentPose = SearchTree(root, bone->mName);

	glm::mat4 currentPoseMatrix = glm::mat4(
		currentPose.a1, currentPose.a2, currentPose.a3, currentPose.a4,
		currentPose.b1, currentPose.b2, currentPose.b3, currentPose.b4,
		currentPose.c1, currentPose.c2, currentPose.c3, currentPose.c4,
		currentPose.d1, currentPose.d2, currentPose.d3, currentPose.d4
		);
	
	//glm::mat4 identity =glm::mat4(1.0f);

	for(unsigned int i = 0; i < bone->mNumWeights; i++)
	{
		float w = boneVertexWeights.at(i);
		glm::vec4 finalVertex = glm::vec4(boneVertices.at(i), 1.0f) * currentPoseMatrix;
		// Green Arrow, Nightwing, Robin, Iron Man
		knee.push_back(finalVertex.x);
		knee.push_back(finalVertex.z);
		knee.push_back(finalVertex.y * -1);
	}
}

void Node::PreOrderTraversal()
{
	int count = 0;
	PreOrder(root, count);
}

void Node::PreOrder(aiNode* node, int count)
{
	count++;
	if(node->mParent == NULL)
	{
		std::cout << "Root: " << node->mName.data << std::endl;
		/*std::cout << "Transformation: " << node->mTransformation.a1 << " " << node->mTransformation.a2 << " " << node->mTransformation.a3 << " " << node->mTransformation.a4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.b1 << " " << node->mTransformation.b2 << " " << node->mTransformation.b3 << " " << node->mTransformation.b4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.c1 << " " << node->mTransformation.c2 << " " << node->mTransformation.c3 << " " << node->mTransformation.c4 << std::endl;

		std::cout << "\t \t" << node->mTransformation.d1 << " " << node->mTransformation.d2 << " " << node->mTransformation.d3 << " " << node->mTransformation.d4 << std::endl;*/
	}
	else
	{
		std::cout << "Parent: " << node->mParent->mName.data << " Level: " << count << std::endl;
		std::cout << "	Node: " << node->mName.data << std::endl;
		/*if(node->mMeshes != 0)
		{
			std::cout << "	Mesh detected" << std::endl;
			std::cout << "	NumMeshes: " << node->mMeshes << std::endl;
		}*/
		std::cout << "\tTransformation: " << node->mTransformation.a1 << " " << node->mTransformation.a2 << " " << node->mTransformation.a3 << " " << node->mTransformation.a4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.b1 << " " << node->mTransformation.b2 << " " << node->mTransformation.b3 << " " << node->mTransformation.b4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.c1 << " " << node->mTransformation.c2 << " " << node->mTransformation.c3 << " " << node->mTransformation.c4 << std::endl;

		std::cout << "\t \t \t" << node->mTransformation.d1 << " " << node->mTransformation.d2 << " " << node->mTransformation.d3 << " " << node->mTransformation.d4 << std::endl;
	}
	
	if(node->mNumChildren > 0)
	{
		for(unsigned int i = 0; i < node->mNumChildren; i++)
			PreOrder(node->mChildren[i], count);
	}
}

aiMatrix4x4 Node::SearchTree(aiNode* node, aiString name)
{
	aiString currentName = node->mName;
	aiMatrix4x4 mat;

	//corresponding node found
	if (name == currentName)
	{
		//std::cout << name.data << " was found" << std::endl;
		aiMatrix4x4 finalTransform = node->mTransformation;
		while(node->mParent != NULL)
		{
			node = node->mParent;
			//finalTransform = finalTransform * node->mTransformation;
			finalTransform = node->mTransformation * finalTransform;
		}
		return finalTransform.Transpose();
	}
	else
	{
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			aiMatrix4x4 mat = SearchTree(node->mChildren[i], name);
			if(mat.a1 != NULL)
				return mat;
		}
	}
	return aiMatrix4x4(	NULL,0,0,0,
						0,0,0,0,
						0,0,0,0,
						0,0,0,0);
}

aiNode* Node::SearchTreeForNode(aiNode* node, aiString name)
{
	aiString currentName = node->mName;
	aiNode* n;
	//corresponding node found
	if (name == currentName)
	{
		return node;
	}
	else
	{
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			aiNode* n = SearchTreeForNode(node->mChildren[i], name);
			if(n != NULL)
				return n;
		}
	}
	return NULL;
}

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

float* Node::GetKnee()
{
	return &knee.at(0);
}

int Node::GetKneeSize()
{
	return knee.size();
}

float* Node::GetBoneMatrix()
{
	return &boneMatricies.at(0);
	//return &boneMatricies.at(0).a1;
	//return glm::value_ptr(boneMatricies.at(0));
}

int Node::GetNumBones()
{
	return numBones;
}
