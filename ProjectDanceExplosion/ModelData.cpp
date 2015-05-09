#include "ModelData.h"

void ModelData::LoadModelData(std::string ModelFile){

	const char* filePath = ModelFile.c_str();
	aiReleaseImport(scene);
	scene = aiImportFile(filePath,
								aiProcess_CalcTangentSpace |
								aiProcess_Triangulate |
								aiProcess_JoinIdenticalVertices |
								aiProcess_SortByPType |
								aiProcess_FixInfacingNormals |
								aiProcess_GenSmoothNormals |
								aiProcess_GenUVCoords |
								aiProcess_SortByPType
								);
	// error checking
	if(!scene)
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "File loaded successfully" <<std::endl;

	int numModels = (scene->mNumMeshes);
	int numTextures = scene->mNumMaterials;
	std::cout << "Number of models in file: " << numModels << std::endl;
	std::cout << "Number of external textures in file: " << numTextures << std::endl;
	std::cout << "Number of embedded textures in file: " << scene->mNumTextures << std::endl;
	std::cout << "Number of animations in file: " << scene->mNumAnimations << std::endl;
	std::cout << ""<< std::endl;

	if(scene->mNumAnimations > 0)
		animCont = AnimationController(scene->mAnimations,scene->mRootNode);
	else
		animCont = AnimationController();

	// Skeleton data
	modelNodes = Node(scene);

	// Clear any previous skin data
	skinList.clear();

	// For every mesh in scene...
	for(int m = 0; m < numModels; m++)
	{
		// Create a Skinning object
		Skinning newSkin = Skinning(scene);
		// Add it to the list of Skinning objects
		skinList.push_back(newSkin);
		skinList.at(m).StoreBoneMeshes(m);
	}
}

void ModelData::Update(){
	// animating skin
	// for every mesh in scene...
	for(unsigned int j = 0; j < scene->mNumMeshes; j++)
	{
		//
		skinList.at(j).ResetChanged();
		// for every bone in mesh
		for(unsigned int i = 0; i < scene->mMeshes[j]->mNumBones; i++)
			skinList.at(j).MoveMeshToWorldSpace(j, i);
		skinList.at(j).StoreFinalVertexData(j);
	}


}