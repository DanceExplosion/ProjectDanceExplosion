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
GLuint basicProgram, particleProgram;
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
SkyBox skybox = SkyBox();


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

// Toggles for drawing the individual systems
bool toggleParticles = false;
bool toggleBones = false;
bool toggleSkin = false;
bool ninja = false;

// Gotta get that time yo
int oldTime = 0;
int currentAnimation = 0;

// Variables for the FPS and Total Particle Count
float final_fps, particle_Count = 0.0f;

// Funciton with the animation splits for the Ninja model
void animationSplit(std::string model)
{
	animationTimes.clear();

	#pragma region Ninja Animations
	if(model == "ninja"){
		ninja = true;
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
	else {
		ninja = false;
	}
	#pragma endregion
}

// Function for loading the models
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
	//std::string file = fileRoot + "Beast/beastedit.ms3d";
	//std::string file = fileRoot + "Ant/ant01Edit.ms3d";
	//std::string file = fileRoot + "TestGuy/test_DirectX.X";
	//std::string file = fileRoot + "Fat/fatdude.x";
	//std::string file = fileRoot + "Zombie/Zombie_Idle02_roar.X";

	std::string file = fileRoot + "NightWingAS/nightwing anim.dae";
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
	animationSplit("ninja");
	animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);

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

// Function to render the Skin of each model
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

		// Texturing
		glEnable(GL_TEXTURE_2D);
		// diffuse texture for model
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skinList.at(i).GetTextureRef());
		// normal texture for model
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skinList.at(i).GetNormalMapRef());

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

// Function to render the bones of the models
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
	glDrawArrays(GL_POINTS, 0, sceneNodes.GetNumBones());
	
	// Unload shader program
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

// Function to render the main scene
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

	//skybox.renderSkybox();

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

	if (!toggleSkin) {
		// Draw skinned bones
		RenderSkin(MVP);
	}

	if (!toggleBones) {
		// Draw skeleton
		RenderBones(MVP);
	}

	// Update Variables with data from ParticleEmitter 
	final_fps = pEmitter.whatIsFPS();
	particle_Count = pEmitter.particleCount() + pEmitter2.particleCount() + pEmitter3.particleCount() + pEmitter4.particleCount();

	if (!toggleParticles) {
		// Draw Emitter
		pEmitter.Draw(view, projection * view);

		// Particle Emitter Updates
		pEmitter.Update(delta, view);
		pEmitter2.Update(delta, view);
		pEmitter3.Update(delta, view);
		pEmitter4.Update(delta, view);
	}

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
void KeyPress(unsigned char key, int x, int y)
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
void CameraControls(int key, int x, int y)
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
	if (!TwEventMouseButtonGLUT(button, dir, x, y))
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

// Setting up initial camera values
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

// Compiling shader code
void initShaders()
{
	ShaderLoader loader;
	basicProgram = loader.CreateProgram("ModelVertexShader.VERT", "ModelFragmentShader.FRAG");
	particleProgram = loader.CreateProgram("ParticleVertexShader.txt", "ParticleFragmentShader.txt");
	skeletonProgram = loader.CreateProgram("NodeVertexShader.txt", "NodeFragmentShader.txt");
	redrawProgram = loader.CreateProgram("RedrawVertexShader.txt", "RedrawFragmentShader.txt");
}

//Function to assign the particle texture
void StoreParticleTextureData()
{
	// Particle Texture Name

	// Initialising DevIL libraries
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
			// Create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &smokeTex);
			glBindTexture(GL_TEXTURE_2D, smokeTex);

			// Texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Bind data to textureRef
			smokeTex = ilutGLBindTexImage();
		}
	}
	else
	{
		// Error message
		std::cout << "failed to load image" << std::endl;
		ILenum error = ilGetError();
		std::cout << "error: " << iluErrorString(error) << std::endl;
	}

	// Check that ptexture is created
	if (glIsTexture(smokeTex))
		std::cout << "particle texture success" << std::endl;

	// Unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
#pragma endregion

	fileRoot = "Models/p.hanna.jpg";

#pragma region  Loading Phil Image
	// Load in texture
	if (ilLoadImage(fileRoot.c_str()))
	{
		// Checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "particle image loaded" << std::endl;
			// Create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &philTex);
			glBindTexture(GL_TEXTURE_2D, philTex);

			// Texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Bind data to textureRef
			philTex = ilutGLBindTexImage();
		}
	}
	else
	{
		// Error message
		std::cout << "failed to load image" << std::endl;
		ILenum error = ilGetError();
		std::cout << "error: " << iluErrorString(error) << std::endl;
	}

	// Check that ptexture is created
	if (glIsTexture(philTex))
		std::cout << "particle texture success" << std::endl;

	// Unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

#pragma endregion

	fileRoot = "Models/Troll-face.png";

#pragma region  Loading Troll Image
	// Load in texture
	if (ilLoadImage(fileRoot.c_str()))
	{
		// Checking relevant data has been loaded
		ILubyte* data = ilGetData();
		if (!data)
			std::cout << "No image data found" << std::endl;
		else
		{
			std::cout << "particle image loaded" << std::endl;
			// Create new texture enum for model
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &trollTex);
			glBindTexture(GL_TEXTURE_2D, trollTex);

			// Texture wrapping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Texture mipmap generation? usage?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Bind data to textureRef
			trollTex = ilutGLBindTexImage();
		}
	}
	else
	{
		// Error message
		std::cout << "failed to load image" << std::endl;
		ILenum error = ilGetError();
		std::cout << "error: " << iluErrorString(error) << std::endl;
	}

	// Check that ptexture is created
	if (glIsTexture(trollTex))
		std::cout << "particle texture success" << std::endl;

	// Unbinding & disabling texturing
	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

#pragma endregion

}

// Function to allow window resizing
void MyReshape(int width, int height){
	glViewport(0, 0, width, height);
	// Set up projection matrix
	projection = glm::perspective(
		45.0f,						// field of view
		(float)(width / height),	// ratio
		0.1f,						// min of view fustrum
		1500.0f						// max of view frustrum
		);
	TwWindowSize(width, height);
}

// Function for searching Model node trees to get node names for particle emitter assigning
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

// Blank display function
static void display() { glutSwapBuffers(); }
	
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

	void TW_CALL scaleUpCallback(void *clientData)
	{
		pEmitter.scaleBufferUp();
	}

	void TW_CALL scaleDownCallback(void *clientData)
	{
		pEmitter.scaleBufferDown();
	}

	// Call that goes to the next stage in the animation
	void TW_CALL nextAnim(void *clientData)
	{
		if (currentAnimation > 0)
			currentAnimation--;
		else
			currentAnimation = animationTimes.size() - 1;
		animCont.SetLoopTime(animationTimes.at(currentAnimation).x, animationTimes.at(currentAnimation).y);
	}

	// Call that goes to the previous stage in the animation
	void TW_CALL prevAnim(void *clientData)
	{
		if (currentAnimation < animationTimes.size() - 1)
			currentAnimation++;
		else
			currentAnimation = 0;
		animCont.SetLoopTime(animationTimes.at(currentAnimation).x, animationTimes.at(currentAnimation).y);
	}

#pragma endregion

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
	TwWindowSize(800, 800);
	// Send window size events to AntTweakBar
	glutReshapeFunc(MyReshape);
	glutDisplayFunc(display);
	
	// send the 'glutGetModifers' function pointer to AntTweakBar
	TwGLUTModifiersFunc(glutGetModifiers);

	// loading shaders
	initShaders();
	// setting up MVP
	initCamera();
	// loading models from file
	LoadModelData();

	/*skybox.loadSkybox("Textures/",
						"jajlands1_ft.jpg",
						"jajlands1_bk.jpg",
						"jajlands1_lf.jpg",
						"jajlands1_rt.jpg",
						"jajlands1_up.jpg",
						"jajlands1_dn.jpg");*/


	StoreParticleTextureData();

#pragma region Model Node Finding

#pragma region Nightwing
	// Strings to be searched in the Node tree
	aiString leftToeBase = aiString("mixamorig_LeftToeBase");
	aiString rightToeBase = aiString("mixamorig_RightToeBase");
	aiString leftHandBase = aiString("mixamorig_LeftHand");
	aiString rightHandBase = aiString("mixamorig_RightHand");
#pragma endregion

	// Assigning a node pointer the value of a node from the node tree
	aiNode* leftFoot = SearchTree(sceneNodes.root, leftToeBase);
	aiNode* rightFoot = SearchTree(sceneNodes.root, rightToeBase);
	aiNode* leftHand = SearchTree(sceneNodes.root, leftHandBase);
	aiNode* rightHand = SearchTree(sceneNodes.root, rightHandBase);

	// Use this to find names of bones in current loaded model
	//node.PreOrderTraversal();
#pragma endregion


	
#pragma region NightWing examples

	//Create a particle emitter
	pEmitter = ParticleEmitter(particleProgram,	// Shader
		leftFoot,								// Paired Node
		smokeTex);								// Texture
	//Emitter 1 Initial Values
	pEmitter.emitterDir[0] = 0;						// X direction
	pEmitter.emitterDir[1] = 1;						// Y Direction
	pEmitter.emitterDir[2] = 0;						// Z Direction
	pEmitter.velocity = -0.004;						// Initial velocity
	pEmitter.p_lifetime = 50;						// Lifetime of Particles
	pEmitter.startScale = 1;						// Initial size
	pEmitter.endScale = .5;							// Size before death
	pEmitter.p_colourStart = glm::vec4(1, 0, 0, 1);	// Start colour, Red
	pEmitter.p_colourEnd = glm::vec4(0, 0, 1, 0);	// End colour, Blue
	pEmitter.angleRange = glm::vec3(0);				// Angle of emitter
	pEmitter.rate = 100;							// Rate of Spawn

	pEmitter2 = ParticleEmitter(particleProgram,	// Shader
		leftHand,									// Paired Node
		smokeTex);									// Texture
	//Emitter 2 Initial Values
	pEmitter2.emitterDir[0] = 0;					// X Direction
	pEmitter2.emitterDir[1] = 1;					// Y Direction
	pEmitter2.emitterDir[2] = 0;					// Z Direction
	pEmitter2.velocity = -0.001;					// Initial Velocity
	pEmitter2.p_lifetime = 15;						// Lifetime of Particles
	pEmitter2.startScale = 0.7;						// Initial size
	pEmitter2.endScale = 0;							// Size before death
	pEmitter2.p_colourStart = glm::vec4(1, 0.5, 0, 1);// Start colour, 
	pEmitter2.p_colourEnd = glm::vec4(0, 0, 0, 0);	// End colour,
	pEmitter2.angleRange = glm::vec3(4);			// Angle of emitter
	pEmitter2.rate = 100;							// Rate of Spawn

	pEmitter3 = ParticleEmitter(particleProgram,	// Shader
		rightFoot,									// Paired Node
		smokeTex);									// Texture
	//Emitter 3 Initial Values
	pEmitter3.emitterDir[0] = 0;					// X Direction
	pEmitter3.emitterDir[1] = 1;					// Y Direction
	pEmitter3.emitterDir[2] = 0;					// Z Direction
	pEmitter3.velocity = -0.004;					// Initial Velocity
	pEmitter3.p_lifetime = 50;						// Lifetime of Particles	
	pEmitter3.startScale = 1;						// Initial size
	pEmitter3.endScale = .5;						// Size before death
	pEmitter3.p_colourStart = glm::vec4(0, 1, 0, 1);// Start colour,
	pEmitter3.p_colourEnd = glm::vec4(1, 0, 0, 0);	// End colour, 
	pEmitter3.angleRange = glm::vec3(0);			// Angle of emitter
	pEmitter3.rate = 100;							// Rate of Spawn

	pEmitter4 = ParticleEmitter(particleProgram,	// Shader
		NULL,										// No paired node
		smokeTex);									// Texture
	//Emitter 4 Initial Values
	pEmitter4.emitterDir[0] = 0;					// X Direction
	pEmitter4.emitterDir[1] = 0;					// Y Direction
	pEmitter4.emitterDir[2] = -1;					// Z Direction
	pEmitter4.position = glm::vec3(5, 0, 0);		// Position
	pEmitter4.velocity = -0.001;					// Initial Velocity
	pEmitter4.p_lifetime = 50;						// Lifetime of Particles
	pEmitter4.startScale = 2;						// Initial size
	pEmitter4.endScale = 0;							// Size before death
	pEmitter4.p_colourStart = glm::vec4(0, 0.7, 1, 1);// Start colour,
	pEmitter4.p_colourEnd = glm::vec4(0.5, 0, 0.5, 0);// End colour, 
	pEmitter4.angleRange = glm::vec3(0.5);			// Angle of emitter
	pEmitter4.rate = 100;							// Rate of Spawn

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
	TwAddVarRO(tBar, "Particle Count", TW_TYPE_FLOAT, &particle_Count, " ");
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
	TwAddSeparator(tBar, NULL, " ");

	TwAddVarRW(tBar, "Toggle Particle Emitter", TW_TYPE_BOOLCPP, &toggleParticles, " ");
	TwAddVarRW(tBar, "Toggle Skin", TW_TYPE_BOOLCPP, &toggleSkin, " ");
	TwAddVarRW(tBar, "Toggle Bones", TW_TYPE_BOOLCPP, &toggleBones, " ");
	TwAddSeparator(tBar, NULL, " ");

	// Check if model is ninja, allow animation control buttons
	if (ninja)
	{
		TwAddButton(tBar, "Next Anim | Ninja", nextAnim, NULL, " help='Only use when Ninja is active' ");
		TwAddButton(tBar, "Prev Anim | Ninja", prevAnim, NULL, " help='Only use when Ninja is active' ");
		TwAddSeparator(tBar, NULL, " ");
	}

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

	
	#pragma region Emitter1 GUI

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

	glutIdleFunc(RenderScene);

	// keyboard control
	glutKeyboardFunc(KeyPress);
	glutSpecialFunc(CameraControls);
	glutMouseFunc(MouseWheel);

	glutMainLoop();
}
