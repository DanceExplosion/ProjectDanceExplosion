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
#include "SkyBox.h"

// OpenGL Essentials
GLuint window;
GLuint basicProgram, particleProgram, nodeProgram;

AnimationController animCont = AnimationController();

ParticleEmitter pEmitter = ParticleEmitter();
ParticleEmitter pEmitter2 = ParticleEmitter();
ParticleEmitter pEmitter3 = ParticleEmitter();
ParticleEmitter pEmitter4 = ParticleEmitter();

Node node = Node();

//SkyBox skybox = SkyBox();

// Camera
glm::mat4 projection;
glm::mat4 view;
int xCircle = 1, yCircle = 1;
float camX = 0, camY = 0, zoom = -4;//-800;
float lightX = 0,lightY = 0,lightZ = 0;

// Models
std::vector<Mesh> modelList;
int numModels;

const aiScene* scene, *animScene;
	Assimp::Importer importer;
	Assimp::Importer importer2;

std::string currentModel;
std::vector<glm::vec2> animationTimes;

GLuint vao;

float final_fps = 0.0f;

// Gotta get that time yo
int oldTime = 0;
int currentAnimation = 0;
void animationSplit(std::string model){
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
	std::string file = fileRoot + "NightWingAS/nightwing anim.dae";
	//std::string file = fileRoot + "Ninja/ninjaEdit.ms3d";
	//std::string file = fileRoot + "Ant/ant01Edit.ms3d";
	//std::string file = fileRoot + "Army Pilot/ArmyPilot.dae";
	//std::string file = fileRoot + "Optimus/Optimus.dae";
	//std::string file = fileRoot + "Robin188/Robin188.dae";
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
	animScene = importer2.ReadFile(animPath, aiProcess_SortByPType);
	// error checking
	if(!scene)
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "File loaded successfully" <<std::endl;
	
	int anim = animScene->mNumAnimations;

	

	numModels = (scene->mNumMeshes);
	int numTextures = scene->mNumMaterials;
	node = Node(scene);
	std::cout << "Number of models in file: " << numModels << std::endl;
	std::cout << "Number of external textures in file: " << numTextures << std::endl;
	std::cout << "Number of embedded textures in file: " << scene->mNumTextures << std::endl;
	std::cout << "Number of animations in file: " << scene->mNumAnimations << std::endl;
	std::cout << ""<< std::endl;

	animCont = AnimationController(animScene->mAnimations,scene->mRootNode);
	//animCont = AnimationController(scene->mAnimations,scene->mRootNode);
	//animationSplit("ninja");
	//animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);
	

	// Pulling required data from scene
	for (int i = 0; i < numModels; i++)
	{
		aiMesh* mesh1 = scene->mMeshes[i];
		aiBone* bone1 = mesh1->mBones[0];
		std::cout << "Number of bones in model "<< i << " : "<< mesh1->mNumBones << std::endl;
		std::cout << "Number of faces in model "<< i << " : "<< mesh1->mNumFaces << std::endl;
		
		if (mesh1->HasTextureCoords(0))
			modelList.push_back(Mesh(mesh1, scene->mMaterials[mesh1->mMaterialIndex]));
		else
			modelList.push_back(Mesh(mesh1, NULL));
	}
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

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

	//Rendering skybox
	//skybox.renderSkybox();

	GLuint vertexBuffer;

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
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

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
	}
	// Unload model shader program, pEmitter uses its own 
	glUseProgram(0);

	
	#pragma region Skeleton Drawing
	// Use skeleton drawing program
	glUseProgram(nodeProgram);

	glDisable(GL_DEPTH_TEST);
	// Draw model skeleton
	GLuint bonebuffer;
	int bufferSize = node.GetNumBones() * 16 * 4; // Every bone has a 4x4 matrix, and every value in the matrix is 4 bytes long
	glGenBuffers(1, &bonebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, &node.StoreBones().at(0), GL_STATIC_DRAW);

	// Bind BasicVertexShader.MVP to this.matrixId
	GLuint MatrixId = glGetUniformLocation(nodeProgram, "MVP");	
	// USING SHADERS
	glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);

	// Pass texture coordinate data
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
		sizeof(float) * 4 * 4,				// stride
		(void*)(sizeof(float) * 4)		// array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glVertexAttribPointer(
		2,				// VertexArrayAttrib
		4,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalised?
		sizeof(float) * 4 * 4,				// stride
		(void*)(sizeof(float) * 8)		// array buffer offset
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

	// Increase the line width and point size, so that the drawn Skeleton is easier to see
	glLineWidth(4.0f);
	glPointSize(5.0f);

	// Draw the Skeleton
	glDrawArrays(GL_POINTS, 0, node.GetNumBones());
	
	// Unload shader program
	glUseProgram(0);
	glDeleteBuffers(1, &bonebuffer);
	glEnable(GL_DEPTH_TEST);

	#pragma endregion

	int timeAtStart = glutGet(GLUT_ELAPSED_TIME);

	// Wibbly wobbly timey-wimey stuff
	int delta = timeAtStart - oldTime;
	oldTime = timeAtStart;
	animCont.Update(delta);

	// Particle Emitter Updates
	pEmitter.Update(delta);
	pEmitter2.Update(delta);
	pEmitter3.Update(delta);
	pEmitter4.Update(delta);

	final_fps = pEmitter.whatIsFPS();

	// Emitter Draw
	pEmitter.Draw(view, projection * view);

	// Emitter Cleanups
	pEmitter.Cleanup();
	pEmitter2.Cleanup();
	pEmitter3.Cleanup();
	pEmitter4.Cleanup();
	
	// delete vao
	glDeleteVertexArrays(1, &vao);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// TweakBar
	
	TwDraw();
	GLenum error = glGetError();
	std::cout << error << std::endl;


	glutSwapBuffers();
}

// Calculate view matrix
void MoveCamera(/*int x, int y*/)
{
	view = glm::lookAt(
		glm::vec3(0, 0, zoom),// camera position in world space
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
	case ',':
		if(currentAnimation > 0)
			currentAnimation--;
		else
			currentAnimation = animationTimes.size()-1;
		animCont.SetLoopTime(animationTimes.at(currentAnimation).x,animationTimes.at(currentAnimation).y);
		break;
	case '.':
		if(currentAnimation < animationTimes.size()-1)
			currentAnimation++;
		else
			currentAnimation = 0;
		animCont.SetLoopTime(animationTimes.at(currentAnimation).x,animationTimes.at(currentAnimation).y);
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
	if (!TwEventMouseButtonGLUT(button, dir, x, y))   // Send event to AntTweakBar
	{
		// Checks if the button pressed is button 3; this is the code for positive wheel scrolling
		if (button == 3)
		{
			zoom += 0.75f;
			MoveCamera();
		}
		// Checks if the button pressed is button 4; this is the code for negative wheel scrolling
		else if (button == 4)
		{
			zoom -= 0.75f;
			MoveCamera();
		}
	}
		
}

// Recursive method for finding the node with a requested name
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
	particleProgram = loader.CreateProgram("ParticleVertexShader.txt", "ParticleFragmentShader.txt");
	nodeProgram = loader.CreateProgram("NodeVertexShader.txt", "NodeFragmentShader.txt");
}

void TW_CALL scaleUpCallback(void *clientData)
{
	pEmitter.scaleBufferUp();
}

void TW_CALL scaleDownCallback(void *clientData)
{
	pEmitter.scaleBufferDown();
}

void main(int argc, char** argv)
{
	// initialising freeglut
	glutInit(&argc, argv);

	// Create TweakBar
	TwBar *tBar;
	TwInit(TW_OPENGL, NULL);

	tBar = TwNewBar("Options");
	TwWindowSize(800, 800);
	TwDefine(" Options size='200 800' color='96 216 224' ");
	TwDefine(" Options position='0 0' ");
	
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


	// loading shaders
	initShaders();
	// setting up MVP
	initCamera();

	// Loading SkyBox Textures	
	/*skybox.loadSkybox("Textures/",
						"jajlands1_ft.jpg",
						"jajlands1_bk.jpg",
						"jajlands1_lf.jpg",
						"jajlands1_rt.jpg",
						"jajlands1_up.jpg",
						"jajlands1_dn.jpg");*/

	// loading models from file
	LoadModelData();

	glutIdleFunc(RenderScene);

	// keyboard control
	// Linked to TweakBar
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	//glutKeyboardFunc(KeyPress);
	glutSpecialFunc(CameraControls);
	glutMouseFunc(MouseWheel);


	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

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
	aiNode* leftFoot = SearchTree(node.root, leftToeBase);
	aiNode* rightFoot = SearchTree(node.root, rightToeBase);
	aiNode* leftHand = SearchTree(node.root, leftHandBase);
	aiNode* rightHand = SearchTree(node.root, rightHandBase);

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
		glm::vec3(0, 0, 0),							// Start Position offset
		glm::vec3(0, 0, 0.0001f),					// Velocity
		glm::vec3(0, 0, 0),							// Accelleration
		10.0f,										// Lifetime
		glm::vec4(255, 255, 255, 225),				// Colour - White
		leftFoot);									// Node to pair with

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

#pragma endregion
		

#pragma region TweakBar

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



#pragma region PEmitter1

#pragma region - Velocity
	TwAddSeparator(tBar, NULL, " group='Emitter 1' ");
	TwAddVarRW(tBar, "Emitter 1 : X Velocity", TW_TYPE_FLOAT, &pEmitter.p_velocity.x, " label='X Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=q keyDecr=Q group='Emitter 1'");
	TwAddVarRW(tBar, "Emitter 1 : Y Velocity", TW_TYPE_FLOAT, &pEmitter.p_velocity.y, " label='Y Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=w keyDecr=W group='Emitter 1'");
	TwAddVarRW(tBar, "Emitter 1 : Z Velocity", TW_TYPE_FLOAT, &pEmitter.p_velocity.z, " label='Z Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=e keyDecr=E group='Emitter 1'");
	TwAddSeparator(tBar, NULL, " group='Emitter 1' ");
	TwAddVarRW(tBar, "Emitter 1 : Lifetime", TW_TYPE_FLOAT, &pEmitter.p_lifetime, " label='Particle Lifetime for Emitter 1' min=0 step=10 keyIncr=r keyDecr=R group='Emitter 1' ");
#pragma endregion

#pragma region - Colour

	TwAddSeparator(tBar, NULL, " group='Emitter 1' ");
	TwAddVarRW(tBar, "E1 Color", TW_TYPE_COLOR4F, &pEmitter.p_colour, " group='Emitter 1'");
	TwAddSeparator(tBar, NULL, " group='Emitter 1' ");



#pragma endregion

#pragma endregion

#pragma region PEmitter2

#pragma region - Velocity
	TwAddSeparator(tBar, NULL, " group='Emitter 2' ");
	TwAddVarRW(tBar, "Emitter 2 : X Velocity", TW_TYPE_FLOAT, &pEmitter2.p_velocity.x, " label='X Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=a keyDecr=A group='Emitter 2'");
	TwAddVarRW(tBar, "Emitter 2 : Y Velocity", TW_TYPE_FLOAT, &pEmitter2.p_velocity.y, " label='Y Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=s keyDecr=S group='Emitter 2'");
	TwAddVarRW(tBar, "Emitter 2 : Z Velocity", TW_TYPE_FLOAT, &pEmitter2.p_velocity.z, " label='Z Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=d keyDecr=D group='Emitter 2'");
	TwAddSeparator(tBar, NULL, " group='Emitter 2' ");
	TwAddVarRW(tBar, "Emitter 2 : Lifetime", TW_TYPE_FLOAT, &pEmitter2.p_lifetime, " label='Particle Lifetime' min=0 step=10 keyIncr=f keyDecr=F group='Emitter 2' ");
#pragma endregion

#pragma region - Colour

	TwAddSeparator(tBar, NULL, " group='Emitter 2' ");
	TwAddVarRW(tBar, "E2 Color", TW_TYPE_COLOR4F, &pEmitter2.p_colour, " group='Emitter 2'");
	TwAddSeparator(tBar, NULL, " group='Emitter 2' ");

#pragma endregion

#pragma endregion

#pragma region PEmitter3

#pragma region - Velocity
	TwAddSeparator(tBar, NULL, " group='Emitter 3' ");
	TwAddVarRW(tBar, "Emitter 3 : X Velocity", TW_TYPE_FLOAT, &pEmitter3.p_velocity.x, " label='X Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=z keyDecr=Z group='Emitter 3'");
	TwAddVarRW(tBar, "Emitter 3 : Y Velocity", TW_TYPE_FLOAT, &pEmitter3.p_velocity.y, " label='Y Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=x keyDecr=X group='Emitter 3'");
	TwAddVarRW(tBar, "Emitter 3 : Z Velocity", TW_TYPE_FLOAT, &pEmitter3.p_velocity.z, " label='Z Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=c keyDecr=C group='Emitter 3'");
	TwAddSeparator(tBar, NULL, " group='Emitter 3' ");
	TwAddVarRW(tBar, "Emitter 3 : Lifetime", TW_TYPE_FLOAT, &pEmitter3.p_lifetime, " label='Particle Lifetime' min=0 step=10 keyIncr=v keyDecr=V group='Emitter 3' ");
#pragma endregion

#pragma region - Colour

	TwAddSeparator(tBar, NULL, " group='Emitter 3' ");
	TwAddVarRW(tBar, "E3 Color", TW_TYPE_COLOR4F, &pEmitter3.p_colour, " group='Emitter 3'");
	TwAddSeparator(tBar, NULL, " group='Emitter 3' ");

#pragma endregion

#pragma endregion

#pragma region PEmitter4

#pragma region - Velocity
	TwAddSeparator(tBar, NULL, " group='Emitter 4' ");
	TwAddVarRW(tBar, "Emitter 4 : X Velocity", TW_TYPE_FLOAT, &pEmitter4.p_velocity.x, " label='X Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=t keyDecr=T group='Emitter 4'");
	TwAddVarRW(tBar, "Emitter 4 : Y Velocity", TW_TYPE_FLOAT, &pEmitter4.p_velocity.y, " label='Y Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=y keyDecr=Y group='Emitter 4'");
	TwAddVarRW(tBar, "Emitter 4 : Z Velocity", TW_TYPE_FLOAT, &pEmitter4.p_velocity.z, " label='Z Velocity' min=-0.01 max=0.01 step=0.0001 keyIncr=u keyDecr=U group='Emitter 4'");
	TwAddSeparator(tBar, NULL, " group='Emitter 4' ");
	TwAddVarRW(tBar, "Emitter 4 : Lifetime", TW_TYPE_FLOAT, &pEmitter4.p_lifetime, " label='Particle Lifetime' min=0 step=10 keyIncr=i keyDecr=I group='Emitter 4' ");
#pragma endregion

#pragma region - Colour

	TwAddSeparator(tBar, NULL, " group='Emitter 4' ");
	TwAddVarRW(tBar, "E4 Color", TW_TYPE_COLOR4F, &pEmitter4.p_colour, " group='Emitter 4'");
	TwAddSeparator(tBar, NULL, " group='Emitter 4' ");

#pragma endregion

#pragma endregion


#pragma endregion

	glutMainLoop();
}