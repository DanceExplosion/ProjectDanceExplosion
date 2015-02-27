#include "Mesh.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

Mesh::Mesh(aiMesh* m, aiMaterial* mat)
{
	numVertices = m->mNumFaces * 3; // more accurate than m->mNumVerticies
	aiNumVertices = m->mNumVertices;
	numFaces = m->mNumFaces;
	modelData = m;

	// check for textures before storing vertex data
	if (mat != NULL)
	{
		std::cout << "textures detected" << std::endl;
		StoreTextureData(mat);
	}

	// will evert model have verticies? or should we check for them?
	StoreVertexData();

	// check for other data and store accordingly
	std::cout << "end of mesh" << std::endl;
	std::cout << "" << std::endl;


	modelData = NULL;
}

void Mesh::StoreVertexData()
{
	// checking for normals
	boolean hasNormals = false;
	if (modelData->HasNormals())
	{
		std::cout << "normals detected" << std::endl;
		hasNormals = true;
	}

	// checking model is triangle based
	if(modelData->mPrimitiveTypes == aiPrimitiveType_TRIANGLE)
		std::cout << "Triangle Model" << std::endl;
	else
		std::cout << "Not triangle model" << std::endl;
	
	// for each face in mesh
	for (int i = 0; i < numFaces; i++)
	{
		const aiFace& currentFace = modelData->mFaces[i];
		// all data for indice
		aiVector3D pos, texture, normal;

		// each face has 3 vertices
		for (int j = 0; j < 3; j++)
		{
			pos = modelData->mVertices[currentFace.mIndices[j]];
			// store vertex.x, y & z in vector
			vertexData.push_back(pos.x);
			vertexData.push_back(pos.y);
			vertexData.push_back(pos.z);

			// if textures exist, store coordinates
			if(textureRef != NULL)
			{
				StoreTextureCoordData(currentFace, j);
			}

			// if normals exist, store them
			if (hasNormals == true)
				StoreNormalData(j);
		}
	}
}

void Mesh::StoreNormalData(int index)
{
	// pull data into vec3
	aiVector3D normal = modelData->mNormals[index];

	// check normalized?
	normal.Normalize();
	// store in vector
	normalData.push_back(normal.x);
	normalData.push_back(normal.y);
	normalData.push_back(normal.z);
}

// assigning 
void Mesh::StoreTextureData(aiMaterial* mat)
{
	std::cout << "Texture Index: " << modelData->mMaterialIndex << std::endl;	

	/*// What kind of textures?
	unsigned int numTextures = mat->GetTextureCount(aiTextureType_DIFFUSE);
	std::cout <<"	Number of diffuse textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_AMBIENT);
	std::cout <<"	Number of ambient textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_DISPLACEMENT);
	std::cout <<"	Number of displacement textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_EMISSIVE);
	std::cout <<"	Number of emmisive textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_EMISSIVE);
	std::cout <<"	Number of emmisive textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_HEIGHT);
	std::cout <<"	Number of height textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_LIGHTMAP);
	std::cout <<"	Number of lightmap textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_NORMALS);
	std::cout <<"	Number of normal textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_OPACITY);
	std::cout <<"	Number of opacity textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_REFLECTION);
	std::cout <<"	Number of reflection textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_SHININESS);
	std::cout <<"	Number of shininess textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_SPECULAR);
	std::cout <<"	Number of specular textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_UNKNOWN);
	std::cout <<"	Number of unknown textures detected: " << numTextures << std::endl;
	numTextures = mat->GetTextureCount(aiTextureType_NONE);
	std::cout <<"	Number of NONE? textures detected: " << numTextures << std::endl;*/

	// static texture loading
	// in two parts for reasons...

	//std::string fileRoot = "Models/Nightwing187";
	std::string fileRoot = "Models/IronMan";
	//std::string fileRoot = "Models/Optimus";
	//std::string fileRoot = "Models/Robin188";
	//std::string fileRoot = "Models/GreenArrow";
	//std::string fileRoot = "Models/Bear_Brown";
	//std::string fileRoot = "Models/C3P0";

	//std::string filePath = fileRoot + "/Nightwing_DIFFUSE.tga";
	std::string filePath = fileRoot + "/Iron_Man_D.tga";
	//std::string filePath = fileRoot + "/Optimus_Diffuse.png";
	//std::string filePath = fileRoot + "/Robin_DIFFUSE.tga";
	//std::string filePath = fileRoot + "/GreenArrow_DIFFUSE.tga";
	//std::string filePath = fileRoot + "/Bear_Fur_D.tga";
	//std::string filePath = fileRoot + "/Bear_D.tga";
	//std::string filePath = fileRoot + "/C3P0_Body_D.tga";

	// initialising DevIL libraries
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

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
			glActiveTexture(GL_TEXTURE0);
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

	// check that ptexture is created
	if(glIsTexture(textureRef))
		std::cout << "texture success" << std::endl;

	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
}

void Mesh::StoreTextureCoordData(aiFace currentFace, int index)
{	
	aiVector3D texture = modelData->mTextureCoords[0][currentFace.mIndices[index]];
	textureCoordData.push_back(texture.x);
	textureCoordData.push_back(texture.y);
}

int Mesh::GetNumFaces()
{
	return numFaces;
}

// as this is calculated when Mesh is created, this is more accurate for this mesh
int Mesh::GetNumVertices()
{
	return numVertices;
}

// number of different vertices in file
int Mesh::GetAINumVertices()
{
	return aiNumVertices;
}

float* Mesh::GetVertexData()
{
	// reference first element of vector
	return &vertexData.at(0);
}

float* Mesh::GetTextureCoords()
{
	return &textureCoordData.at(0);
}

GLuint Mesh::GetTextureData()
{
	return textureRef;
}

float* Mesh::GetNormalData()
{
	return &normalData.at(0);
}