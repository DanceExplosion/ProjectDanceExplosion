#include "Skinning.h"

Skinning::Skinning() {}

Skinning::Skinning(const aiScene* mS)
{
	nodesAllocated = false;
	// Store the pointer to the scene
	mainScene = mS;
	// root node of scene
	root = mainScene->mRootNode;
}

void Skinning::StoreBoneMeshes(int meshIndex)
{
	aiBone* bone;
	aiMesh* mesh = mainScene->mMeshes[meshIndex];
	nodeReferences.resize(mesh->mNumBones);
	// Store texture
	StoreTextureData(mainScene->mMaterials[mesh->mMaterialIndex]);

	// for every bone in that mesh...
	for(unsigned int i = 0; i < mainScene->mMeshes[meshIndex]->mNumBones; i++)
		{
			bone = mesh->mBones[i];

			nodeReferences.at(i) = FindNode(root, bone->mName);
			// create a new Mesh object
			BoneMesh currentMesh = BoneMesh(mesh, i);

			// Store Mesh vertices in Bone Space
			currentMesh.StoreBoneSpaceVertices();

			//Store Mesh Object
			modelMeshes.push_back(currentMesh);
		}

	// worldSpaceVertices is the same size as boneSpaceVertices
	worldSpaceVertices.resize(mesh->mNumVertices);
	// guarentee textureCoordData is empty
	textureCoordData.clear();
	// fill Changed with FALSE
	ResetChanged();
}

void Skinning::MoveMeshToWorldSpace(int meshIndex, int boneIndex)
{
	aiMesh* mesh = mainScene->mMeshes[meshIndex];
	aiBone* bone = mesh->mBones[boneIndex];
	// get current pose from node
	aiMatrix4x4 currentPose = GetTotalTransform(nodeReferences.at(boneIndex));
	
	BoneMesh currentMesh = modelMeshes.at(boneIndex);

	// for every vertex in aiBone...
	for(unsigned int v = 0; v < bone->mNumWeights; v++)
	{		
		// vertices in BoneMesh are stored in the same order as they appear in aiBone->mWeights
		aiVector3D currentVertex = currentMesh.GetVertexAt(v); // vertex in boneSpace
		unsigned int vertexIndex = bone->mWeights[v].mVertexId;
		float weight = bone->mWeights[v].mWeight;
		
		
		//convert currentvertex to glm format
		glm::vec3 finalVertex = glm::vec3(currentVertex.x, currentVertex.y, currentVertex.z);

		// cast to aiVector for multiplication
		currentVertex = aiVector3D(finalVertex.x,finalVertex.y,finalVertex.z);

		// Move the vertex out of bind pose and into local space
		aiTransformVecByMatrix4(&currentVertex, &(bone->mOffsetMatrix));

		// Transform the vertex by the node's total transformation to get to the bone's position
		aiTransformVecByMatrix4(&currentVertex, &currentPose);

		// Convert to vec3 from glm, for multiplication and storage
		finalVertex = glm::vec3(currentVertex.x, currentVertex.y, currentVertex.z);

		if(changed.at(vertexIndex) == FALSE)
		{
			worldSpaceVertices.at(vertexIndex) = ((finalVertex)*glm::vec3(weight,weight,weight));
			changed.at(vertexIndex) = TRUE;
		}
		else
		{
			worldSpaceVertices.at(vertexIndex) += (finalVertex)*glm::vec3(weight, weight,weight);
		}
	}

}

void Skinning::StoreFinalVertexData(int meshIndex)
{
	//clear all data first
	finalVertexData.clear();

	aiMesh* modelData = mainScene->mMeshes[meshIndex];
	int numFaces = modelData->mNumFaces;
	bool recordTexture = textureCoordData.empty();

	// for each face in mesh...
	for (int i = 0; i < numFaces; i++)
	{
		const aiFace& currentFace = modelData->mFaces[i];
		glm::vec3 pos;

		// each face has 3 vertices
		for (int j = 0; j < 3; j++)
		{
			pos = worldSpaceVertices.at(currentFace.mIndices[j]);

			// store vertex.x, y & z in vector
			finalVertexData.push_back(glm::vec3(pos.x, pos.y, pos.z));
			
			// if texture coordinates not already stored 
			if(recordTexture)
				// store texture co-ordinates
 				StoreTextureCoordData(modelData, currentFace, j);
		}
	}
}

void Skinning::StoreTextureCoordData(aiMesh* modelData, aiFace currentFace, int index)
{	
	int n = currentFace.mIndices[index];
	aiVector3D texture = modelData->mTextureCoords[0][currentFace.mIndices[index]];
	glm::vec2 textCo = glm::vec2(texture.x, texture.y);
	textureCoordData.push_back(textCo);
}

// Auto-texture detection
void Skinning::StoreTextureData(aiMaterial* mat)
{
	std::string fileRoot = "Models/";

	aiString path;
	std::string filePath;
	// If model file contains a pointer to texture file
	if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		filePath = fileRoot + path.data;
	else
	{
		std::cout << "No textures detected" << std::endl;
		return;
	}

	std::cout << "texture filePath: " << filePath << std::endl;

	// Read file given by model
	filePath = ReadFile(filePath);
	// 
	std::string textureFilePath = fileRoot;

	// Find the filepath of the 1st texture
	// Remove '=' from string
	std::size_t pos = filePath.find_first_of('=') + 1;
	std::size_t pos2 = filePath.find_first_of('\r', pos);	
	// Substr returns partial filepath for texture, appended to textureFilePath to give full path
	textureFilePath += filePath.substr(pos, pos2 - pos);

	// Assuming this is the DIFFUSE texture
	StoreDiffuseTextureData(textureFilePath);

	// if texture file contains more than a DIFFUSE
	if(filePath.length() >= pos2)
	{
		// Reset textureFilePath
		textureFilePath = fileRoot;
		// Find next filepath
		pos = filePath.find_first_of('=', pos) + 1;
		pos2 = filePath.find_first_of('\r', pos);
		// Substr returns partial filepath for texture, appended to textureFilePath to give full path
		textureFilePath += filePath.substr(pos, pos2 - pos);

		// Assuming this is the NORMAL texture
		StoreNormalMapData(textureFilePath);
	}
}

// Read .MAT file
std::string Skinning::ReadFile(std::string filePath)
{
	std::string line;
	std::ifstream matFile (filePath, std::ios::binary);
	if (matFile.is_open())
	{
		std::cout << "File contents:" << std::endl;
		std::string readLine;
		while ( getline (matFile, readLine) )
		{
			line.append(readLine);
			std::cout << "\t" + readLine << std::endl;
		}
		matFile.close();
	}

	else 
		std::cout << "Unable to open file" ; 

	return line;
}

void Skinning::StoreDiffuseTextureData(std::string filePath)
{
	// Assume DIFFUSE is 1st texture (this seems to be the standard with most of the models we've encounterer)
	diffuseTextureRef = LoadTexture(filePath, GL_TEXTURE0);

	// check that ptexture is created
	if(glIsTexture(diffuseTextureRef))
		std::cout << "diffuse texture success" << std::endl;
}

void Skinning::StoreNormalMapData(std::string filePath)
{
	// Assume NORMAL is 2nd texture (this seems to be the standard with most of the models we've encounterer)
	normalMapRef = LoadTexture(filePath, GL_TEXTURE1);

	// check that ptexture is created
	if(glIsTexture(normalMapRef))
	{
		std::cout << "normal texture success" << std::endl;
		std::cout << "" << std::endl;
	}

	// unbinding & disabling texturing
	/*glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);*/
}

GLuint Skinning::LoadTexture(std::string filePath, GLenum activeTexture)
{
	// initialising DevIL libraries
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	GLuint textureRef;

	// Load in texture
	if(ilLoadImage(filePath.c_str()))
	{
		// checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if(!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "image loaded" << std::endl;
			// create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(activeTexture);
			glGenTextures(1, &textureRef);	
			glBindTexture(GL_TEXTURE_2D, textureRef);

			// texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// bind data to textureRef
			textureRef = ilutGLBindTexImage();
		}
	}
	else
	{
		// erro message
		std::cout << "failed to load image" << std::endl;
		ILenum error = ilGetError();
		std::cout << "error: " << iluErrorString(error) << std::endl;
	}

	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

	return textureRef;
}

// Returns total mTransformation of Node and all it's ancestors
aiMatrix4x4 Skinning::GetTotalTransform(aiNode* node)
{
	
		// Store the transformation
		aiMatrix4x4 finalTransform = node->mTransformation;

		// Get the total transformation of the node
		while(node->mParent != NULL)
		{
			aiMatrix4x4 temp = node->mParent->mTransformation;
			finalTransform = temp*finalTransform;
			node = node->mParent;
		}
		//Return the total matrix
		return finalTransform;
}

// Recursive method for finding the node with a requested name
aiNode* Skinning::FindNode(aiNode* node, aiString name)
{
	// Store the name for easy access
	aiString currentName = node->mName;

	// Matrix to be returned
	aiMatrix4x4 mat;

	// Corresponding node found
	if (name == currentName)
	{
		//Return the node reference
		return node;
	}
	// If the name doesn't match
	else
	{
		// Loop through all of the current node's children
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			// Get the return value of the recursive search
			aiNode* mat = FindNode(node->mChildren[i], name);

			// If it has returned a value that isn't NULL, return it
			if(mat != NULL)
				return mat;
		}
	}
	// If nothing has been found, then this branch of the skeleton is a dead end, return NULL for the first value to represent this
	return NULL;
}

void Skinning::ResetChanged()
{
	changed.assign(worldSpaceVertices.size(), 0);
}

int Skinning::GetNumVertices()
{
	return finalVertexData.size();
}

float* Skinning::GetWorldSpaceVertices()
{
	return &finalVertexData.at(0).x;
}

float* Skinning::GetTextureCoordinates()
{
	return &textureCoordData.at(0).x;
}

GLuint Skinning::GetTextureRef()
{
	return diffuseTextureRef;
}

GLuint Skinning::GetNormalMapRef()
{
	return normalMapRef;
}