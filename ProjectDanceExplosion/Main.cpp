//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp
#include <assimp/Importer.hpp>	// c++ importer interface
#include <assimp/scene.h>		// output data structure
#include <assimp/postprocess.h>	// post processing flag


// OpenGL Mathemathics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Extra Libraries
#include <iostream>
#include <AntTweakBar.h>

// Inner Project Classes
#include "ShaderLoader.h"
#include "Mesh.h"
#include "ParticleEMitter.h"
#include "Node.h"
#include "ModelAnimation.h"
#include "Skinning.h"
#include "SkyBox.h"

// OpenGL Essentials
GLuint window;
GLuint basicProgram, particleProgram, skyboxProgram;
GLuint redrawProgram, skeletonProgram;

// Animation
AnimationController animCont = AnimationController();
std::vector<glm::vec2> animationTimes;

// Particle Emitters
ParticleEmitter pEmitter = ParticleEmitter();
ParticleEmitter pEmitter2 = ParticleEmitter();
ParticleEmitter pEmitter3 = ParticleEmitter();
ParticleEmitter pEmitter4 = ParticleEmitter();

GLuint smokeTex,philTex,trollTex;

Node sceneNodes = Node();
std::vector<Skinning> skinList;
SkyBox skybox;


// Camera
glm::mat4 projection;
glm::mat4 view;
int xCircle = 1, yCircle = 1;
float camX = 0, camY = 0, zoom = -10;
float lightX = 0,lightY = 0,lightZ = 0;

// Models
int numModels;

const aiScene* scene, *animScene;
	Assimp::Importer importer;
	Assimp::Importer importer2;

GLuint vao;

// Gotta get that time yo
int oldTime = 0;
int currentAnimation = 0;
float final_fps = 0.0f;

void animationSplit(std::string model)
{
	animationTimes.clear();

	#pragma region Ninja Animations
	if(model == "ninja"){
		// Standing
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(104),
								animCont.currentAnimation->mDuration));

		// Walking
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(0),
										animCont.getFrameTime(7)));
		
		// Sneaking
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(8),
										animCont.getFrameTime(14)));

		// Attacking
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(16),
										animCont.getFrameTime(24)));

		// Attack 2
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(24),
										animCont.getFrameTime(27)));

		// Kick
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(28),
										animCont.getFrameTime(37)));
		
		// Pickup
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(38),
										animCont.getFrameTime(46)));

		// Jump with motion
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(47),
										animCont.getFrameTime(53)));

		// Jump with no motion
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(54),
										animCont.getFrameTime(60)));

		// Jump strike
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(60),
										animCont.getFrameTime(69)));
		
		// Kick 2
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(69),
										animCont.getFrameTime(74)));
		
		// Spin attack
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(74),
										animCont.getFrameTime(80)));
		
		// Backflip
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(80),
										animCont.getFrameTime(85)));
		
		// Climb/Freak out
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(87),
										animCont.getFrameTime(89)));
		
		// Die
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(90),
										animCont.getFrameTime(92)));
		
		// Faceplant
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(93),
										animCont.getFrameTime(96)));
		// Weird standing
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(97),
										animCont.getFrameTime(104)));
		
	}
	#pragma endregion
	
	#pragma region Beast Animations
	else if(model == "beast"){
		
		// Idle
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(90),
										animCont.getFrameTime(95)));

		// Walking
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(0),
										animCont.getFrameTime(5)));

		// Surfacing
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(6),
										animCont.getFrameTime(16)));
		
		// Head bob
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(16),
										animCont.getFrameTime(20)));

		// Jump
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(24),
										animCont.getFrameTime(30)));
		
		// Roar
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(35),
										animCont.getFrameTime(42)));
		
		// Look around
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(43),
										animCont.getFrameTime(51)));
		
		// Look around 2
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(80),
										animCont.getFrameTime(89)));

		// Headbutt
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(51),
										animCont.getFrameTime(55)));

		// Bite
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(55),
										animCont.getFrameTime(62)));
		
		// Bite 2
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(62),
										animCont.getFrameTime(68)));
		
		// Stomp
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(68),
										animCont.getFrameTime(75)));

		// Hit
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(75),
										animCont.getFrameTime(80)));

		// Death
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(95),
										animCont.currentAnimation->mDuration));
	}
		#pragma endregion
	else{
		// If the model isn't one of the ones listed above, just play the enitre animation
		animationTimes.push_back(glm::vec2(animCont.getFrameTime(0),
								animCont.currentAnimation->mDuration));
	}
}

void LoadModelData()
{
	// set background colour //black
	glClearColor(0.3f, 0.3f, 0.6f, 0.0f);

	// Assimp file importer
	std::string fileRoot = "Models/";
	//std::string file = fileRoot + "Bear_Brown/Bear_Brown.dae";
	//std::string file = fileRoot + "C3P0/C3P0.dae";
	//std::string file = fileRoot + "Dog/Dog.dae";
	//std::string file = fileRoot + "GreenArrow/GreenArrow.dae";
	//std::string file = fileRoot + "IronMan/Iron_Man.dae";
	//std::string file = fileRoot + "Nightwing187/Nightwing187.dae";
	//std::string file = fileRoot + "Optimus/Optimus.dae";
	//std::string file = fileRoot + "Robin188/Robin188.dae";

	//std::string file = fileRoot + "Ninja/ninjaEdit.ms3d";
	//std::string file = fileRoot + "Primal Beast/PrimalBeast.ms3d";
	//std::string file = fileRoot + "Beast/beast.ms3d";
	std::string file = fileRoot + "Beast/beastedit.ms3d";
	//std::string file = fileRoot + "Ant/ant01Edit.ms3d";
	//std::string file = fileRoot + "TestGuy/test_DirectX.X";
	//std::string file = fileRoot + "Fat/fatdude.x";
	//std::string file = fileRoot + "Zombie/Zombie_Idle02_roar.X";

	//std::string file = fileRoot + "NightWingAS/nightwing anim.dae";
	//std::string file = fileRoot + "Army Pilot/ArmyPilot.dae";

	std::string animation = fileRoot+"NightWingAS/anim.BVH";
	//std::string animation = fileRoot+"Army Pilot/ArmyPilot.BVH";

	const char* filePath = file.c_str();
	const char* animPath = animation.c_str();
	scene = importer.ReadFile(filePath,
								aiProcess_CalcTangentSpace |
								aiProcess_Triangulate |
								aiProcess_JoinIdenticalVertices |
								aiProcess_SortByPType |
								aiProcess_FixInfacingNormals |
								aiProcess_GenSmoothNormals |
								aiProcess_GenUVCoords |
								aiProcess_SortByPType
								);
	animScene = importer2.ReadFile(animPath, aiProcess_CalcTangentSpace |
								aiProcess_Triangulate |
								aiProcess_JoinIdenticalVertices |
								aiProcess_SortByPType |
								aiProcess_FixInfacingNormals |
								aiProcess_GenSmoothNormals |
								aiProcess_GenUVCoords |
								aiProcess_SortByPType);
	// error checking
	if(!scene)
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "File loaded successfully" <<std::endl;
	
	int anim = animScene->mNumAnimations;	

	numModels = (scene->mNumMeshes);
	int numTextures = scene->mNumMaterials;
	std::cout << "Number of models in file: " << numModels << std::endl;
	std::cout << "Number of external textures in file: " << numTextures << std::endl;
	std::cout << "Number of embedded textures in file: " << scene->mNumTextures << std::endl;
	std::cout << "Number of animations in file: " << scene->mNumAnimations << std::endl;
	std::cout << ""<< std::endl;

	animCont = AnimationController(scene->mAnimations,scene->mRootNode);
	animationSplit("beast");
	animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);

	// Pulling required data from scene
	/*for (int i = 0; i < numModels; i++)
	{
		aiMesh* mesh1 = scene->mMeshes[i];
		aiBone* bone1 = mesh1->mBones[0];
		std::cout << "Number of bones in model "<< i << " : "<< mesh1->mNumBones << std::endl;
		std::cout << "Number of faces in model "<< i << " : "<< mesh1->mNumFaces << std::endl;
		
		if (mesh1->HasTextureCoords(0))
			modelList.push_back(Mesh(mesh1, scene->mMaterials[mesh1->mMaterialIndex]));
		else
			modelList.push_back(Mesh(mesh1, NULL));
	}*/

	// Skinning
	sceneNodes = Node(scene);
	// for every mesh in scene...
	for(int m = 0; m < numModels; m++)
	{
		// create a Skinning object
		Skinning newSkin = Skinning(scene);
		skinList.push_back(newSkin);
		skinList.at(m).StoreBoneMeshes(m);

		//
		for(unsigned int i = 0; i < scene->mMeshes[m]->mNumBones; i++)
		{
			skinList.at(m).MoveMeshToWorldSpace(m, i);
		}
		skinList.at(m).ResetChanged();
		skinList.at(m).StoreFinalVertexData(m);
		std::cout << "" << std::endl;
	}

}

void RenderSkin(glm::mat4 MVP)
{
	// Use skeleton drawing program
	glUseProgram(redrawProgram);

	for(int i = 0; i < numModels; i++)
	{
		// vertices
		GLuint vertexBuffer;
		int bufferSize = skinList.at(i).GetNumVertices() * 3 * 4; // each vertex has 3 parts (x, y & z), each part is 4 bytes long
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, skinList.at(i).GetWorldSpaceVertices(), GL_STATIC_DRAW);

		// textures
		bufferSize = skinList.at(i).GetNumVertices() * 2 * 4; // each texture coordinate has 2 parts(x & y), each part is 4 bytes long
		GLuint texturebuffer;
		glGenBuffers(1, &texturebuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, skinList.at(i).GetTextureCoordinates(), GL_STATIC_DRAW);

		// bind RedrawVertexShader.MVP to this.matrixId
		GLuint MatrixId = glGetUniformLocation(redrawProgram, "MVP");	
		// bind RedrawFragmentShader.diffuseSampler to this.diffuseSampler
		GLuint diffuseSampler = glGetUniformLocation(redrawProgram, "diffuseSampler");
		// bind RedrawFragmentShader.normalSampler to this.normalSampler
		GLuint normalSampler = glGetUniformLocation(redrawProgram, "normalSampler");
		// USING SHADERS
		glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(diffuseSampler, 0);
		glUniform1i(normalSampler, 1);

		// Texturing
		glEnable(GL_TEXTURE_2D);
		// diffuse texture for model
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skinList.at(i).GetTextureRef());
		// normal texture for model
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skinList.at(i).GetNormalMapRef());

		// pass vertex data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,				// VertexArrayAttrib
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		// pass texture coordinate data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glVertexAttribPointer(
			1,				// VertexArrayAttrib
			2,				// size
			GL_FLOAT,		// type
			GL_TRUE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		int numVertices = skinList.at(i).GetNumVertices();
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		// disable texturing
		glBindTexture(GL_TEXTURE_2D, NULL);
		glActiveTexture(NULL);
		glDisable(GL_TEXTURE_2D);

		// disable and delete vertexbuffer and texturebuffer
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &texturebuffer);
	}

	// Unload shader program
	glUseProgram(0);
}

void RenderBones(glm::mat4 MVP)
{
	// Use skeleton drawing program
	glUseProgram(skeletonProgram);

	glDisable(GL_DEPTH_TEST);
	// Draw model skeleton
	GLuint bonebuffer;
	int bufferSize = sceneNodes.GetNumBones() * 16 * 4; // every bone has a 4x4 matrix, and every value in the matrix is 4 bytes long
	glGenBuffers(1, &bonebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, &sceneNodes.StoreBones().at(0), GL_STATIC_DRAW);

	//bind BasicVertexShader.MVP to this.matrixId
	GLuint MatrixId = glGetUniformLocation(skeletonProgram, "MVP");	
	// USING SHADERS
	glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);

	// pass texture coordinate data
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glVertexAttribPointer(
		0,				// VertexArrayAttrib
		4,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalised?
		sizeof(float) * 4 * 4,				// stride
		(void*)0		// array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glVertexAttribPointer(
		1,				// VertexArrayAttrib
		4,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalised?
		sizeof(float) * 4 * 4,		// stride
		(void*)(sizeof(float) * 4)	// array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glVertexAttribPointer(
		2,				// VertexArrayAttrib
		4,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalised?
		sizeof(float) * 4 * 4,		// stride
		(void*)(sizeof(float) * 8)	// array buffer offset
		);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glVertexAttribPointer(
		3,				// VertexArrayAttrib
		4,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalised?
		sizeof(float) * 4 * 4,				// stride
		(void*)(sizeof(float) * 12)		// array buffer offset
		);

	glLineWidth(4.0f);
	glPointSize(5.0f);
	//glDrawArrays(GL_LINES, 0, sceneNodes.GetNumBones());
	//glDrawArrays(GL_LINE_STRIP, 0, sceneNodes.GetNumBones());
	//glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, sceneNodes.GetNumBones());
	//glDrawArrays(GL_LINE_LOOP, 0, sceneNodes.GetNumBones());
	//glDrawArrays(GL_LINES_ADJACENCY, 0, sceneNodes.GetNumBones());
	glDrawArrays(GL_POINTS, 0, sceneNodes.GetNumBones());
	
	glDeleteBuffers(1, &bonebuffer);
	// Unload shader program
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// model's model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	// calculating MVP
	glm::mat4 MVP = projection * view * modelMatrix;

	glm::mat4 MV = view * modelMatrix;

	// set shader program
	glUseProgram(basicProgram);	
	
	// buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Bind various matrices to the shader
	GLuint matrixId = glGetUniformLocation(basicProgram, "MVP");
	GLuint modelMId = glGetUniformLocation(basicProgram, "M");
	GLuint viewMId = glGetUniformLocation(basicProgram, "V");
	GLuint modelViewMId = glGetUniformLocation(basicProgram, "MV");

	// Get the light position
	GLuint lightPosId = glGetUniformLocation(basicProgram, "LightPosition_worldspace");

	// Pass in the coordinated for the light
	glUniform3f(lightPosId, lightX,lightY,lightZ);

	// USING SHADERS
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);
	
	glUniformMatrix4fv(modelMId, 1, GL_FALSE, &modelMatrix[0][0]);
	
	glUniformMatrix4fv(viewMId, 1, GL_FALSE, &view[0][0]);
	
	glUniformMatrix4fv(modelViewMId, 1, GL_FALSE, &MV[0][0]);

	skybox.renderSkybox(view, projection);

/*	GLuint vertexBuffer;

	for(int i = 0; i < numModels; i++)
	{
		// vertex buffer		
		int bufferSize = modelList.at(i).GetNumVertices() * 3 * 4; // each vertex has 3 parts(x, y & z), each part is 4 bytes long
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, modelList.at(i).GetVertexData(), GL_STATIC_DRAW);

		// texture buffer
		bufferSize = modelList.at(i).GetNumVertices() * 2 * 4; // each texture coordinate has 2 parts(x & y), each part is 4 bytes long
		GLuint texturebuffer;
		glGenBuffers(1, &texturebuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, modelList.at(i).GetTextureCoords(), GL_STATIC_DRAW);
		
		// normal buffer
		bufferSize = modelList.at(i).GetNumVertices() * 3 * 4; // each normal has 3 parts(x, y & z), each part is 4 bytes long
		GLuint normalbuffer;
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, modelList.at(i).GetNormalData(), GL_STATIC_DRAW);

		// Texturing
		// diffuse texture for model
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, modelList.at(i).GetTextureData());

		// normal texture for model
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, modelList.at(i).GetNormalMapData());
		// bind fragmentShader.textureSampler to this.textureSampler
		GLuint normalSampler = glGetUniformLocation(basicProgram, "normalSampler");
		// pass in texture data
		glUniform1i(normalSampler, 1);


		// pass vertex data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,				// VertexArrayAttrib
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		// pass texture coordinate data
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glVertexAttribPointer(
			1,				// VertexArrayAttrib
			2,				// size
			GL_FLOAT,		// type
			GL_TRUE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		// pass texture coordinate data
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,				// VertexArrayAttrib
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		// draw model
		int numVertices = modelList.at(i).GetNumVertices();
		//glDrawArrays(GL_TRIANGLES, 0, numVertices);

		// Texturing
		glBindTexture(GL_TEXTURE_2D, NULL);
		glActiveTexture(NULL);

		// disable & delete vbo, texturebuffer & normalbuffer
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &texturebuffer);
		glDeleteBuffers(1, &normalbuffer);
	}*/
	// Unload model shader program, pEmitter uses its own 
	glUseProgram(0);

	int timeAtStart = glutGet(GLUT_ELAPSED_TIME);

	// Wibbly wobbly timey-wimey stuff
	int delta = timeAtStart - oldTime;
	oldTime = timeAtStart;
	animCont.Update(delta);

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

	// draw skinned bones
	RenderSkin(MVP);
	// draw skeleton
	RenderBones(MVP);

	// Particle Emitter Updates
	pEmitter.Update(delta, view);
	pEmitter2.Update(delta,view);
	pEmitter3.Update(delta, view);
	pEmitter4.Update(delta, view);

	// Emitter Draw
	pEmitter.Draw(view, projection * view);

	// Emitter Cleanups
	pEmitter.Cleanup();
	pEmitter2.Cleanup();
	pEmitter3.Cleanup();
	pEmitter4.Cleanup();
	
	// menu refresh
	TwDraw();
	// OpenGL refrepsh
	glutSwapBuffers();
		
	// delete vao
	glDeleteVertexArrays(1, &vao);
	glDisable(GL_DEPTH_TEST);
}

// Calculate view matrix
void MoveCamera(/*int x, int y*/)
{
	view = glm::lookAt(
		glm::vec3(0,0, zoom),// camera position in world space
		glm::vec3(0, 0, 0), // where camera is viewing in world space
		glm::vec3(0, 1, 0)  // Y is up (in world space)
		);
	view = glm::rotate(view, camX, glm::vec3(0.0f,1.0f,0.0f));
	view = glm::rotate(view, camY, glm::vec3(1.0f,0.0f,0.0f));

	// redraw scene immediately
	glutPostRedisplay();
}

// Keyboard controls
void KeyPress(unsigned char key, int x, int y )
{
	switch (key)
	{
	case 27: // close application by pressing "Esc"
		glutDestroyWindow(window);
		exit(0);
		break;
	case 'w':
		lightY++;
		break;
	case 's':
		lightY--;
		break;
	case 'd':
		lightX++;
		break;
	case 'a':
		lightX--;
		break;
	case 'r':
		lightZ++;
		break;
	case 'f':
		lightZ--;
		break;
	case 'g':
		pEmitter.particleCount();
		break;
	case 'h':
		pEmitter.disable();
		break;
	case 'j':
		pEmitter.whatIsFPS();
		break;
	default:
		break;
	}
}

// Keyboard controls for camera movement
void CameraControls(int key, int x , int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camY += 0.3f;
		MoveCamera();
		break;
	case GLUT_KEY_DOWN:
		camY -= 0.3f;
		MoveCamera();
		break;
	case GLUT_KEY_RIGHT:
		camX += 0.3f;
		MoveCamera();
		break;
	case GLUT_KEY_LEFT:
		camX -= 0.3f;
		MoveCamera();
		break;
	case GLUT_KEY_SHIFT_L:
		zoom += 0.75f;
		MoveCamera();
		break;
	case GLUT_KEY_CTRL_L:
		zoom -= 0.75f;
		MoveCamera();
		break;
	default:
		break;
	}
}

// Mouse Controll for Zooming [a lot handier than dealing with Sticky based keys for windows
void MouseWheel(int button, int dir, int x, int y)
{
	if( !TwEventMouseButtonGLUT(button, dir, x, y) )
	{
		// Checks if the button pressed is button 3; this is the code for positive wheel scrolling
		if (button == 3)
			zoom += 0.75f;
		// Checks if the button pressed is button 4; this is the code for negative wheel scrolling
		else if (button == 4)
			zoom -= 0.75f;

		MoveCamera();
	}
		
}

// setting up initial camera values
void initCamera()
{
	// projection matrix
	projection = glm::perspective(
		45.0f,			// field of view
		4.0f / 4.0f,	// ratio
		0.1f,			// min of view fustrum
		1500.0f			// max of view frustrum
		);

	// camY needs a default value
	camY = 0.0f;
	// view matrix
	MoveCamera();
}

// compiling shader code
void initShaders()
{
	ShaderLoader loader;
	basicProgram = loader.CreateProgram("ModelVertexShader.VERT", "ModelFragmentShader.FRAG");
	skyboxProgram = loader.CreateProgram("SkyboxVertexShader.VERT", "SkyboxFragmentShader.FRAG");
	particleProgram = loader.CreateProgram("ParticleVertexShader.txt", "ParticleFragmentShader.txt");
	skeletonProgram = loader.CreateProgram("NodeVertexShader.txt", "NodeFragmentShader.txt");
	redrawProgram = loader.CreateProgram("RedrawVertexShader.txt", "RedrawFragmentShader.txt");
}

void StoreParticleTextureData()
{
	//Particle Texture Name
	
	// initialising DevIL libraries
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	std::string fileRoot = "Models/SmokeShape.png";

	#pragma region  Loading Smoke Image


	// Load in texture
	if (ilLoadImage(fileRoot.c_str()))
	{
		// checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "particle image loaded" << std::endl;
			// create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &smokeTex);
			glBindTexture(GL_TEXTURE_2D, smokeTex);

			// texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// bind data to textureRef
			smokeTex = ilutGLBindTexImage();
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
	if (glIsTexture(smokeTex))
		std::cout << "particle texture success" << std::endl;
	
	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
#pragma endregion
	
	fileRoot = "Models/p.hanna.jpg";
	
	#pragma region  Loading Phil Image
	// Load in texture
	if (ilLoadImage(fileRoot.c_str()))
	{
		// checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "particle image loaded" << std::endl;
			// create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &philTex);
			glBindTexture(GL_TEXTURE_2D, philTex);

			// texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// bind data to textureRef
			philTex = ilutGLBindTexImage();
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
	if (glIsTexture(philTex))
		std::cout << "particle texture success" << std::endl;

	
	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
#pragma endregion
	
	fileRoot = "Models/Troll-face.png";
	
	#pragma region  Loading Troll Image
	// Load in texture
	if (ilLoadImage(fileRoot.c_str()))
	{
		// checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "particle image loaded" << std::endl;
			// create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &trollTex);
			glBindTexture(GL_TEXTURE_2D, trollTex);

			// texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// bind data to textureRef
			trollTex = ilutGLBindTexImage();
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
	if (glIsTexture(trollTex))
		std::cout << "particle texture success" << std::endl;
	// unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
#pragma endregion

}

void TW_CALL scaleUpCallback(void *clientData)
{
	pEmitter.scaleBufferUp();
}

void TW_CALL scaleDownCallback(void *clientData)
{
	pEmitter.scaleBufferDown();
}

void MyReshape(int width, int height){
	glViewport(0,0,width,height);
	// Set up projection matrix
	projection = glm::perspective(
		45.0f,						// field of view
		(float)(width / height),	// ratio
		0.1f,						// min of view fustrum
		1500.0f						// max of view frustrum
		);
	TwWindowSize(width, height);
}

aiNode* SearchTree(aiNode* node, aiString name)
{
	// Store the name for easy access
	aiString currentName = node->mName;

	// Matrix to be returned
	aiMatrix4x4 mat;

	// Corresponding node found
	if (name == currentName)
	{
		//Return the total matrix
		return node;
	}
	// If the name doesn't match
	else
	{
		// Loop through all of the current node's children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			// Get the return value of the recursive search
			aiNode* mat = SearchTree(node->mChildren[i], name);

			// If it has returned a value that isn't NULL, return it
			if (mat != NULL)
				return mat;
		}
	}

	// If nothing has been found, then this branch of the skeleton is a dead end, return NULL for the first value to represent this
	return NULL;
}


static void display() {}
	
#pragma region Emitter Button Functions
	// Change Emitter's graphic to Smoke
	void TW_CALL emitSmoke(void *clientData)
	{ 
		pEmitter.textureRef = smokeTex;
	}

	// Change Emitter's graphic to Phil
	void TW_CALL emitPhil(void *clientData)
	{ 
		pEmitter.textureRef = philTex;
	}

	// Change Emitter's graphic to a trollface
	void TW_CALL emitTroll(void *clientData)
	{ 
		pEmitter.textureRef = trollTex;
	}

#pragma endregion

	void CloseFunction(){
		TwTerminate();
	}
void main(int argc, char** argv)
{
	// initialising freeglut
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(700, 100);// window position
	glutInitWindowSize(800, 800); // window size
	window = glutCreateWindow("Project DanceExplosion");
	// GLEW must be intialized after the window has been created
	GLenum glewOK = glewInit();
	if (!glewOK == GLEW_OK)
		std::cout << "glew error: " << glewGetErrorString(glewOK) << std::endl;
	else
		std::cout << "glew running" << std::endl;

	// directly redirect GLUT events (excluding mouse) to AntTweakBar
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);

	TwInit(TW_OPENGL, NULL);
	// Send window size events to AntTweakBar
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(display);
	//TwWindowSize(800, 800);
	
	// send the 'glutGetModifers' function pointer to AntTweakBar
	TwGLUTModifiersFunc(glutGetModifiers);

	// loading shaders
	initShaders();
	// setting up MVP
	initCamera();
	// loading models from file
	LoadModelData();
	
	skybox.loadSkybox(skyboxProgram, "Textures/",
						"lake1_ft.jpg",
						"lake1_bk.jpg",
						"lake1_lf.jpg",
						"lake1_rt.jpg",
						"lake1_up.jpg",
						"lake1_dn.jpg");

	StoreParticleTextureData();

#pragma region Model Node Finding

#pragma region Nightwing
	// Strings to be searched in the Node tree
	aiString leftToeBase = aiString("mixamorig_LeftToeBase");
	aiString rightToeBase = aiString("mixamorig_RightToeBase");
	aiString leftHandBase = aiString("mixamorig_LeftHand");
	aiString rightHandBase = aiString("mixamorig_RightHand");
#pragma endregion

	aiString joint = aiString("Joint6");
	aiString joint2 = aiString("Joint5");


#pragma endregion

	// Assigning a node pointer the value of a node from the node tree
	aiNode* leftFoot = SearchTree(sceneNodes.root, leftToeBase);
	aiNode* rightFoot = SearchTree(sceneNodes.root, rightToeBase);
	aiNode* leftHand = SearchTree(sceneNodes.root, leftHandBase);
	aiNode* rightHand = SearchTree(sceneNodes.root, rightHandBase);

	//aiNode* yes = SearchTree(node.root, joint);
	//aiNode* no = SearchTree(node.root, joint2);

	// Use this to find names of bones in current loaded model
	//node.PreOrderTraversal();

#pragma region Iron Man examples
	/*
	
	//Feet Thruster Particles
	//Create a particle emitter
	pEmitter = ParticleEmitter(particleProgram,		// Shader
		glm::vec3(0.2f, 0, 0),						// Start Position
		glm::vec3(0, 0, 0.01),						// Velocity
		glm::vec3(0, 0, -0.004f),					// Accelleration
		170.0f,										// Lifetime
		glm::vec4(255, 255, 255, 255));				// Colour

	//Create a second particle emitter
	pEmitter2 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(-0.2f, 0, 0),						// Start Position
		glm::vec3(0, 0, 0.01),						// Velocity
		glm::vec3(0, 0, -0.0004f),					// Accelleration
		170.0f,										// Lifetime
		glm::vec4(0, 255, 255, 255));				// Colour

	// Hand Thruster Particles
	//Create a second particle emitter
	pEmitter3 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(1.8f, 0, 2.9f),					// Start Position
		glm::vec3(0, 0, 0.01),						// Velocity
		glm::vec3(0, 0, -0.0004f),					// Accelleration
		170.0f,										// Lifetime
		glm::vec4(255, 255, 0, 255));					// Colour

	//Create a second particle emitter
	pEmitter4 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(-1.8f, 0, 2.9f),					// Start Position
		glm::vec3(0, 0, 0.01),						// Velocity
		glm::vec3(0, 0, -0.0004f),					// Accelleration
		170.0f,										// Lifetime
		glm::vec4(255, 0, 255, 255));				// Colour
		*/
#pragma endregion

	
#pragma region NightWing examples

	//Create a particle emitter
	pEmitter = ParticleEmitter(particleProgram,	// Shader
		leftFoot,
		smokeTex);									// Node to pair with
	//Emitter 1 Initial Values
	pEmitter.emitterDir[0] = 0;
	pEmitter.emitterDir[1] = 1;
	pEmitter.emitterDir[2] = 0;
	pEmitter.velocity = 0.004;
	pEmitter.p_lifetime = 50;
	pEmitter.startScale = 1;
	pEmitter.endScale = .5;
	pEmitter.p_colourStart = glm::vec4(1,0,0,1);
	pEmitter.p_colourEnd = glm::vec4(0,0,1,0);
	pEmitter.angleRange = glm::vec3(0);
	pEmitter.rate = 100;

	pEmitter2 = ParticleEmitter(particleProgram,	// Shader
		leftHand,
		smokeTex);									// Node to pair with
									// Node to pair with
	//Emitter 2 Initial Values
	pEmitter2.emitterDir[0] = 0;
	pEmitter2.emitterDir[1] = 1;
	pEmitter2.emitterDir[2] = 0;
	pEmitter2.velocity = 0.001;
	pEmitter2.p_lifetime = 15;
	pEmitter2.startScale = 0.7;
	pEmitter2.endScale = 0;
	pEmitter2.p_colourStart = glm::vec4(1,0.5,0,1);
	pEmitter2.p_colourEnd = glm::vec4(0,0,0,0);
	pEmitter2.angleRange = glm::vec3(4);
	pEmitter2.rate = 100;

	pEmitter3 = ParticleEmitter(particleProgram,	// Shader
		rightFoot,
		smokeTex);									// Node to pair with
	//Emitter 3 Initial Values
	pEmitter3.emitterDir[0] = 0;
	pEmitter3.emitterDir[1] = 1;
	pEmitter3.emitterDir[2] = 0;
	pEmitter3.velocity = 0.004;
	pEmitter3.p_lifetime = 50;
	pEmitter3.startScale = 1;
	pEmitter3.endScale = .5;
	pEmitter3.p_colourStart = glm::vec4(0,1,0,1);
	pEmitter3.p_colourEnd = glm::vec4(1,0,0,0);
	pEmitter3.angleRange = glm::vec3(0);
	pEmitter3.rate = 100;

	
	pEmitter4 = ParticleEmitter(particleProgram,	// Shader
		NULL,
		smokeTex);									// Node to pair with
	//Emitter 4 Initial Values
	pEmitter4.emitterDir[0] = 0;
	pEmitter4.emitterDir[1] = 0;
	pEmitter4.emitterDir[2] = -1;
	pEmitter4.position = glm::vec3(5,0,0);
	pEmitter4.velocity = 0.001;
	pEmitter4.p_lifetime = 50;
	pEmitter4.startScale = 2;
	pEmitter4.endScale = 0;
	pEmitter4.p_colourStart = glm::vec4(0,0.7,1,1);
	pEmitter4.p_colourEnd = glm::vec4(0.5,0,0.5,0);
	pEmitter4.angleRange = glm::vec3(0.5);
	pEmitter4.rate = 100;

	/*
	//Create a particle emitter
	pEmitter2 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(0, 0, 0),							// Start Position offset
		glm::vec3(0, 0, 0.0001f),					// Velocity
		glm::vec3(0, 0, 0),							// Accelleration
		10.0f,										// Lifetime
		glm::vec4(0, 255, 0, 255),					// Colour - Magenta
		rightFoot);									// Node to pair with


	//Create a particle emitter
	pEmitter3 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(0, 0, 0),							// Start Position offset
		glm::vec3(0, 0, 0.0001f),					// Velocity
		glm::vec3(0, 0, 0),							// Accelleration
		10.0f,										// Lifetime
		glm::vec4(0, 0, 255, 255),
		leftHand);

	//Create a particle emitter
	pEmitter4 = ParticleEmitter(particleProgram,	// Shader
		glm::vec3(0, 0, 0),							// Start Position offset
		glm::vec3(0, 0, 0.0001f),					// Velocity
		glm::vec3(0, 0, 0),							// Accelleration
		10.0f,										// Lifetime
		glm::vec4(255, 0, 0, 255),
		rightHand);					// Color
	*/
#pragma endregion

#pragma region TweakBar
	
	// Create TweakBar
	TwBar *tBar;

	tBar = TwNewBar("GlobalOptions");
	TwDefine("GlobalOptions size='200 300'" /*color='96 216 224' "*/);
	TwDefine("GlobalOptions position='10 10' ");

	// FPS in UI
	TwAddVarRO(tBar, "FPS", TW_TYPE_FLOAT, &final_fps, " ");
	TwAddSeparator(tBar, NULL, " ");

	// Button to call function to Scale the Particles bigger
	TwAddButton(tBar, "ScaleBuffer Up", scaleUpCallback, NULL, " group='Scaling' ");

	// Button to call function to Scale the particles smaller
	TwAddButton(tBar, "ScaleBuffer Down", scaleDownCallback, NULL, " group='Scaling' ");
	TwAddSeparator(tBar, NULL, " ");

	// Button to call function to Scale the particles smaller
	TwAddVarRW(tBar, "Toggle Additive Blending", TW_TYPE_BOOLCPP, &pEmitter.additive, " group='Blending' ");
	TwAddSeparator(tBar, NULL, " ");

	TwAddButton(tBar, "Emit Smoke", emitSmoke, NULL, " label='Emit Smoke'");
	TwAddButton(tBar, "Emit Phil", emitPhil, NULL, " label='Emit Phil'");
	TwAddButton(tBar, "Emit Troll", emitTroll, NULL, " label='Emit Troll'");

	// GUI Setup
	
	#pragma region Emitter1 GUI

	// Window that will store the settings for the emitter and the particles
	TwBar *emitterBar1;
	emitterBar1 = TwNewBar("Emitter1Settings");
	TwDefine("Emitter1Settings  size='250 500' iconified=false position='530 20' ");

	// Variables stored in the Particle's section of the window
	#pragma region Particle Variables
	// Particle Velocity and Velocity range
	TwAddVarRW(emitterBar1, "Velocity", TW_TYPE_FLOAT, &pEmitter.velocity, "step=0.0001 group='Particle Properties'");
	TwAddVarRW(emitterBar1, "Velocity Range", TW_TYPE_FLOAT, &pEmitter.velocityRange, "step=0.0001 group='Particle Properties'");
	// Particle Lifetime
	TwAddVarRW(emitterBar1, "Life Span", TW_TYPE_FLOAT, &pEmitter.p_lifetime, "group='Particle Properties' step=1 min=2");
	// Particle Start and end Size
	TwAddVarRW(emitterBar1, "Initial Size", TW_TYPE_FLOAT, &pEmitter.startScale, "group='Particle Properties' step=0.1");
	TwAddVarRW(emitterBar1, "Ending Size", TW_TYPE_FLOAT, &pEmitter.endScale, "group='Particle Properties' step=0.1");

	// Particle Accelleration
	TwAddVarRW(emitterBar1, "Acceleration X", TW_TYPE_FLOAT, &pEmitter.p_acc.x, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar1, "Acceleration Y", TW_TYPE_FLOAT, &pEmitter.p_acc.y, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar1, "Acceleration Z", TW_TYPE_FLOAT, &pEmitter.p_acc.z, "group='Acceleration' step=0.00001");

	// Particle Angle Range
	TwAddVarRW(emitterBar1, "Angle Range X", TW_TYPE_FLOAT, &pEmitter.angleRange.x, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar1, "Angle Range Y", TW_TYPE_FLOAT, &pEmitter.angleRange.y, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar1, "Angle Range Z", TW_TYPE_FLOAT, &pEmitter.angleRange.z, "group='AngleRange' step=0.1");

	// Add grouped items to the particle section
	TwDefine("Emitter1Settings/AngleRange  group='Particle Properties'  ");
	TwDefine("Emitter1Settings/Acceleration  group='Particle Properties'  ");

	// Particle start and end colour
	TwAddVarRW(emitterBar1, "Start Colour", TW_TYPE_COLOR4F, &pEmitter.p_colourStart, "group='Particle Properties'");
	TwAddVarRW(emitterBar1, "End Colour", TW_TYPE_COLOR4F, &pEmitter.p_colourEnd, "group='Particle Properties'");
	#pragma endregion

	// Variables stored in the Emitter's section of the window
	#pragma region Emitter Variables
	// Emitter emission rate
	TwAddVarRW(emitterBar1, "Rate", TW_TYPE_FLOAT, &pEmitter.rate, "step=0.5 group='Emitter Properties' min=0");
	// Main direction of the Emitter
	TwAddVarRW(emitterBar1, "Emitter Angle", TW_TYPE_DIR3F, &pEmitter.emitterDir, "group='Emitter Properties'");

	// Location in worldspace of the emitter
	TwAddVarRW(emitterBar1, "PositionX", TW_TYPE_FLOAT, &pEmitter.position.x, "group='Position' step=0.1");
	TwAddVarRW(emitterBar1, "PositionY", TW_TYPE_FLOAT, &pEmitter.position.y, "group='Position' step=0.1");
	TwAddVarRW(emitterBar1, "PositionZ", TW_TYPE_FLOAT, &pEmitter.position.z, "group='Position' step=0.1");

	// Add the position group to the emitter section
	TwDefine("Emitter1Settings/Position  group='Emitter Properties'  ");

	#pragma endregion

	#pragma endregion

	#pragma region Emitter2 GUI

	// Window that will store the settings for the emitter and the particles
	TwBar *emitterBar2;
	emitterBar2 = TwNewBar("emitter2Settings");
	TwDefine("emitter2Settings  size='250 500' iconified=true  ");

	// Variables stored in the Particle's section of the window
	#pragma region Particle Variables
	// Particle Velocity and Velocity range
	TwAddVarRW(emitterBar2, "Velocity", TW_TYPE_FLOAT, &pEmitter2.velocity, "step=0.0001 group='Particle Properties'");
	TwAddVarRW(emitterBar2, "Velocity Range", TW_TYPE_FLOAT, &pEmitter2.velocityRange, "step=0.0001 group='Particle Properties'");
	// Particle Lifetime
	TwAddVarRW(emitterBar2, "Life Span", TW_TYPE_FLOAT, &pEmitter2.p_lifetime, "group='Particle Properties' step=1 min=2");
	// Particle Start and end Size
	TwAddVarRW(emitterBar2, "Initial Size", TW_TYPE_FLOAT, &pEmitter2.startScale, "group='Particle Properties' step=0.1");
	TwAddVarRW(emitterBar2, "Ending Size", TW_TYPE_FLOAT, &pEmitter2.endScale, "group='Particle Properties' step=0.1");

	// Particle Accelleration
	TwAddVarRW(emitterBar2, "Acceleration X", TW_TYPE_FLOAT, &pEmitter2.p_acc.x, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar2, "Acceleration Y", TW_TYPE_FLOAT, &pEmitter2.p_acc.y, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar2, "Acceleration Z", TW_TYPE_FLOAT, &pEmitter2.p_acc.z, "group='Acceleration' step=0.00001");

	// Particle Angle Range
	TwAddVarRW(emitterBar2, "Angle Range X", TW_TYPE_FLOAT, &pEmitter2.angleRange.x, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar2, "Angle Range Y", TW_TYPE_FLOAT, &pEmitter2.angleRange.y, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar2, "Angle Range Z", TW_TYPE_FLOAT, &pEmitter2.angleRange.z, "group='AngleRange' step=0.1");

	// Add grouped items to the particle section
	TwDefine("emitter2Settings/AngleRange  group='Particle Properties'  ");
	TwDefine("emitter2Settings/Acceleration  group='Particle Properties'  ");

	// Particle start and end colour
	TwAddVarRW(emitterBar2, "Start Colour", TW_TYPE_COLOR4F, &pEmitter2.p_colourStart, "group='Particle Properties'");
	TwAddVarRW(emitterBar2, "End Colour", TW_TYPE_COLOR4F, &pEmitter2.p_colourEnd, "group='Particle Properties'");
	#pragma endregion

	// Variables stored in the Emitter's section of the window
	#pragma region Emitter Variables
	// Emitter emission rate
	TwAddVarRW(emitterBar2, "Rate", TW_TYPE_FLOAT, &pEmitter2.rate, "step=0.5 group='Emitter Properties' min=0");
	// Main direction of the Emitter
	TwAddVarRW(emitterBar2, "Emitter Angle", TW_TYPE_DIR3F, &pEmitter2.emitterDir, "group='Emitter Properties'");

	// Location in worldspace of the emitter
	TwAddVarRW(emitterBar2, "PositionX", TW_TYPE_FLOAT, &pEmitter2.position.x, "group='Position' step=0.1");
	TwAddVarRW(emitterBar2, "PositionY", TW_TYPE_FLOAT, &pEmitter2.position.y, "group='Position' step=0.1");
	TwAddVarRW(emitterBar2, "PositionZ", TW_TYPE_FLOAT, &pEmitter2.position.z, "group='Position' step=0.1");

	// Add the position group to the emitter section
	TwDefine("emitter2Settings/Position  group='Emitter Properties'  ");

	#pragma endregion

	#pragma endregion

	
	#pragma region Emitter3 GUI

	// Window that will store the settings for the emitter and the particles
	TwBar *emitterBar3;
	emitterBar3 = TwNewBar("Emitter3Settings");
	TwDefine("Emitter3Settings  size='250 500' iconified=true  ");

	// Variables stored in the Particle's section of the window
	#pragma region Particle Variables
	// Particle Velocity and Velocity range
	TwAddVarRW(emitterBar3, "Velocity", TW_TYPE_FLOAT, &pEmitter3.velocity, "step=0.0001 group='Particle Properties'");
	TwAddVarRW(emitterBar3, "Velocity Range", TW_TYPE_FLOAT, &pEmitter3.velocityRange, "step=0.0001 group='Particle Properties'");
	// Particle Lifetime
	TwAddVarRW(emitterBar3, "Life Span", TW_TYPE_FLOAT, &pEmitter3.p_lifetime, "group='Particle Properties' step=1 min=2");
	// Particle Start and end Size
	TwAddVarRW(emitterBar3, "Initial Size", TW_TYPE_FLOAT, &pEmitter3.startScale, "group='Particle Properties' step=0.1");
	TwAddVarRW(emitterBar3, "Ending Size", TW_TYPE_FLOAT, &pEmitter3.endScale, "group='Particle Properties' step=0.1");

	// Particle Accelleration
	TwAddVarRW(emitterBar3, "Acceleration X", TW_TYPE_FLOAT, &pEmitter3.p_acc.x, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar3, "Acceleration Y", TW_TYPE_FLOAT, &pEmitter3.p_acc.y, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar3, "Acceleration Z", TW_TYPE_FLOAT, &pEmitter3.p_acc.z, "group='Acceleration' step=0.00001");

	// Particle Angle Range
	TwAddVarRW(emitterBar3, "Angle Range X", TW_TYPE_FLOAT, &pEmitter3.angleRange.x, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar3, "Angle Range Y", TW_TYPE_FLOAT, &pEmitter3.angleRange.y, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar3, "Angle Range Z", TW_TYPE_FLOAT, &pEmitter3.angleRange.z, "group='AngleRange' step=0.1");

	// Add grouped items to the particle section
	TwDefine("Emitter3Settings/AngleRange  group='Particle Properties'  ");
	TwDefine("Emitter3Settings/Acceleration  group='Particle Properties'  ");

	// Particle start and end colour
	TwAddVarRW(emitterBar3, "Start Colour", TW_TYPE_COLOR4F, &pEmitter3.p_colourStart, "group='Particle Properties'");
	TwAddVarRW(emitterBar3, "End Colour", TW_TYPE_COLOR4F, &pEmitter3.p_colourEnd, "group='Particle Properties'");
	#pragma endregion

	// Variables stored in the Emitter's section of the window
	#pragma region Emitter Variables
	// Emitter emission rate
	TwAddVarRW(emitterBar3, "Rate", TW_TYPE_FLOAT, &pEmitter3.rate, "step=0.5 group='Emitter Properties' min=0");
	// Main direction of the Emitter
	TwAddVarRW(emitterBar3, "Emitter Angle", TW_TYPE_DIR3F, &pEmitter3.emitterDir, "group='Emitter Properties'");

	// Location in worldspace of the emitter
	TwAddVarRW(emitterBar3, "PositionX", TW_TYPE_FLOAT, &pEmitter3.position.x, "group='Position' step=0.1");
	TwAddVarRW(emitterBar3, "PositionY", TW_TYPE_FLOAT, &pEmitter3.position.y, "group='Position' step=0.1");
	TwAddVarRW(emitterBar3, "PositionZ", TW_TYPE_FLOAT, &pEmitter3.position.z, "group='Position' step=0.1");

	// Add the position group to the emitter section
	TwDefine("Emitter3Settings/Position  group='Emitter Properties'  ");

	#pragma endregion

	#pragma endregion

	#pragma region Emitter4 GUI

	// Window that will store the settings for the emitter and the particles
	TwBar *emitterBar4;
	emitterBar4 = TwNewBar("Emitter4Settings");
	TwDefine("Emitter4Settings  size='250 500' iconified=true  ");

	// Variables stored in the Particle's section of the window
	#pragma region Particle Variables
	// Particle Velocity and Velocity range
	TwAddVarRW(emitterBar4, "Velocity", TW_TYPE_FLOAT, &pEmitter4.velocity, "step=0.0001 group='Particle Properties'");
	TwAddVarRW(emitterBar4, "Velocity Range", TW_TYPE_FLOAT, &pEmitter4.velocityRange, "step=0.0001 group='Particle Properties'");
	// Particle Lifetime
	TwAddVarRW(emitterBar4, "Life Span", TW_TYPE_FLOAT, &pEmitter4.p_lifetime, "group='Particle Properties' step=1 min=2");
	// Particle Start and end Size
	TwAddVarRW(emitterBar4, "Initial Size", TW_TYPE_FLOAT, &pEmitter4.startScale, "group='Particle Properties' step=0.1");
	TwAddVarRW(emitterBar4, "Ending Size", TW_TYPE_FLOAT, &pEmitter4.endScale, "group='Particle Properties' step=0.1");

	// Particle Accelleration
	TwAddVarRW(emitterBar4, "Acceleration X", TW_TYPE_FLOAT, &pEmitter4.p_acc.x, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar4, "Acceleration Y", TW_TYPE_FLOAT, &pEmitter4.p_acc.y, "group='Acceleration' step=0.00001");
	TwAddVarRW(emitterBar4, "Acceleration Z", TW_TYPE_FLOAT, &pEmitter4.p_acc.z, "group='Acceleration' step=0.00001");

	// Particle Angle Range
	TwAddVarRW(emitterBar4, "Angle Range X", TW_TYPE_FLOAT, &pEmitter4.angleRange.x, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar4, "Angle Range Y", TW_TYPE_FLOAT, &pEmitter4.angleRange.y, "group='AngleRange' step=0.1");
	TwAddVarRW(emitterBar4, "Angle Range Z", TW_TYPE_FLOAT, &pEmitter4.angleRange.z, "group='AngleRange' step=0.1");

	// Add grouped items to the particle section
	TwDefine("Emitter4Settings/AngleRange  group='Particle Properties'  ");
	TwDefine("Emitter4Settings/Acceleration  group='Particle Properties'  ");

	// Particle start and end colour
	TwAddVarRW(emitterBar4, "Start Colour", TW_TYPE_COLOR4F, &pEmitter4.p_colourStart, "group='Particle Properties'");
	TwAddVarRW(emitterBar4, "End Colour", TW_TYPE_COLOR4F, &pEmitter4.p_colourEnd, "group='Particle Properties'");
	#pragma endregion

	// Variables stored in the Emitter's section of the window
	#pragma region Emitter Variables
	// Emitter emission rate
	TwAddVarRW(emitterBar4, "Rate", TW_TYPE_FLOAT, &pEmitter4.rate, "step=0.5 group='Emitter Properties' min=0");
	// Main direction of the Emitter
	TwAddVarRW(emitterBar4, "Emitter Angle", TW_TYPE_DIR3F, &pEmitter4.emitterDir, "group='Emitter Properties'");

	// Location in worldspace of the emitter
	TwAddVarRW(emitterBar4, "PositionX", TW_TYPE_FLOAT, &pEmitter4.position.x, "group='Position' step=0.1");
	TwAddVarRW(emitterBar4, "PositionY", TW_TYPE_FLOAT, &pEmitter4.position.y, "group='Position' step=0.1");
	TwAddVarRW(emitterBar4, "PositionZ", TW_TYPE_FLOAT, &pEmitter4.position.z, "group='Position' step=0.1");

	// Add the position group to the emitter section
	TwDefine("Emitter4Settings/Position  group='Emitter Properties'  ");

	#pragma endregion
	
	#pragma endregion

	#pragma endregion
	
	glutIdleFunc(RenderScene);

	// keyboard control
	glutKeyboardFunc(KeyPress);
	glutSpecialFunc(CameraControls);
	glutMouseFunc(MouseWheel);
	glutCloseFunc(CloseFunction);
	glutMainLoop();
	TwTerminate();
}
