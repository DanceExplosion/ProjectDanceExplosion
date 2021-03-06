//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

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
#include "ModelData.h"

// OpenGL Essentials
GLuint window;
GLuint particleProgram, skyboxProgram;
GLuint redrawProgram, skeletonProgram;

std::vector<glm::vec2> animationTimes;

// Particle Emitters
ParticleEmitter pEmitter = ParticleEmitter();
ParticleEmitter pEmitter2 = ParticleEmitter();
ParticleEmitter pEmitter3 = ParticleEmitter();
ParticleEmitter pEmitter4 = ParticleEmitter();

SkyBox skybox = SkyBox();

TwBar *tBar;

// Camera
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 projection;
glm::mat4 view;
float UIQuat[4] = {0,0,0,1};
int xCircle = 1, yCircle = 1;
float camX = 0, camY = 0, zoom = 10;
float lightX = 0,lightY = 0,lightZ = 0;

ModelData loadedModel;

GLuint vao;

// Toggles for drawing the individual systems
bool toggleParticles = true;
bool toggleBones = true;
bool toggleSkin = true;
bool toggleSkybox = true;
bool multipleAnimations = false;
int modelCycle = 0;

// Gotta get that time yo
int oldTime = 0;
int currentAnimation = 0;

// Node locations for Nightwing's attached emitters
aiNode* leftFoot;
aiNode* rightFoot;
aiNode* leftHand;
aiNode* rightHand;

// Variables for the FPS and Total Particle Count
float final_fps, particle_Count = 0.0f;

// Funciton with the animation splits for the Ninja and Beast models
void animationSplit(std::string model)
{
	// Clear whatever times were previously loaded.
	animationTimes.clear();

	// Animation split for the ninja model
	#pragma region Ninja Animations
	if(model == "ninja"){

		// Show animation controls
		multipleAnimations = true;

		// Standing
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(104),
								loadedModel.animCont.currentAnimation->mDuration));

		// Walking
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(0),
										loadedModel.animCont.getFrameTime(7)));
		
		// Sneaking
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(8),
										loadedModel.animCont.getFrameTime(14)));

		// Attacking
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(16),
										loadedModel.animCont.getFrameTime(24)));

		// Attack 2
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(24),
										loadedModel.animCont.getFrameTime(27)));

		// Kick
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(28),
										loadedModel.animCont.getFrameTime(37)));
		
		// Pickup
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(38),
										loadedModel.animCont.getFrameTime(46)));

		// Jump with motion
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(47),
										loadedModel.animCont.getFrameTime(53)));

		// Jump with no motion
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(54),
										loadedModel.animCont.getFrameTime(60)));

		// Jump strike
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(60),
										loadedModel.animCont.getFrameTime(69)));
		
		// Kick 2
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(69),
										loadedModel.animCont.getFrameTime(74)));
		
		// Spin attack
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(74),
										loadedModel.animCont.getFrameTime(80)));
		
		// Backflip
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(80),
										loadedModel.animCont.getFrameTime(85)));
		
		// Climb/Freak out
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(87),
										loadedModel.animCont.getFrameTime(89)));
		
		// Die
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(90),
										loadedModel.animCont.getFrameTime(92)));
		
		// Faceplant
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(93),
										loadedModel.animCont.getFrameTime(96)));
		// Weird standing
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(97),
										loadedModel.animCont.getFrameTime(104)));
		
	#pragma endregion

	// Animation split for the beast model
	#pragma region Beast Animations
	}else if(model == "beast"){
		// Show animation controls
		multipleAnimations = true;

		// Idle
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(89),
										loadedModel.animCont.getFrameTime(93)));

		// Walking
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(0),
										loadedModel.animCont.getFrameTime(5)));

		// Surfacing
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(6),
										loadedModel.animCont.getFrameTime(16)));
		
		// Jumping
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(24),
										loadedModel.animCont.getFrameTime(30)));

		// Roar
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(35),
										loadedModel.animCont.getFrameTime(42)));

		// Crouch and look
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(43),
										loadedModel.animCont.getFrameTime(50)));

		// Stand and look
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(81),
										loadedModel.animCont.getFrameTime(88)));

		// Headbutt
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(51),
										loadedModel.animCont.getFrameTime(56)));

		// Bite
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(56),
										loadedModel.animCont.getFrameTime(62)));

		// Bite 2
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(62),
										loadedModel.animCont.getFrameTime(68)));

		// Stomp
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(68),
										loadedModel.animCont.getFrameTime(74)));

		// Hurt
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(74),
										loadedModel.animCont.getFrameTime(81)));

		// Dead
		animationTimes.push_back(glm::vec2(loadedModel.animCont.getFrameTime(93),
										loadedModel.animCont.currentAnimation->mDuration));
		#pragma endregion
	
		// If the model is neither Beast nor Ninja
	}else {
		// Set the only animation as the entire cycle
		animationTimes.push_back(glm::vec2(0,loadedModel.animCont.currentAnimation->mDuration));
		// Hide the animation controls
		multipleAnimations = false;
	}
}

// Function for loading the models
void LoadModelData()
{
	// Assimp file importer
	std::string fileRoot = "Models/";
	//std::string file = fileRoot + "Bear_Brown/Bear_Brown.dae";
	//std::string file = fileRoot + "C3P0/C3P0.dae";
	//std::string file = fileRoot + "Dog/Dog.dae";
	//std::string file = fileRoot + "GreenArrow/GreenArrow.dae";
	std::string file = fileRoot + "IronMan/Iron_Man.dae";
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

	//std::string file = fileRoot + "NightWingAS/nightwing anim.dae";
	//std::string file = fileRoot + "Army Pilot/ArmyPilot.dae";

	loadedModel.LoadModelData(file);

	//animationSplit("beast");
	//loadedModel.animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);


}

// Function to render the Skin of each model
void RenderSkin(glm::mat4 MVP)
{
	// Use skeleton drawing program
	glUseProgram(redrawProgram);

	for(int i = 0; i < loadedModel.skinList.size(); i++)
	{
		// vertices
		GLuint vertexBuffer;
		int bufferSize = loadedModel.skinList.at(i).GetNumVertices() * 3 * 4; // each vertex has 3 parts (x, y & z), each part is 4 bytes long
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, loadedModel.skinList.at(i).GetWorldSpaceVertices(), GL_STATIC_DRAW);

		// textures
		bufferSize = loadedModel.skinList.at(i).GetNumVertices() * 2 * 4; // each texture coordinate has 2 parts(x & y), each part is 4 bytes long
		GLuint texturebuffer;
		glGenBuffers(1, &texturebuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, loadedModel.skinList.at(i).GetTextureCoordinates(), GL_STATIC_DRAW);

		// Texturing
		glEnable(GL_TEXTURE_2D);
		// diffuse texture for model
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, loadedModel.skinList.at(i).GetTextureRef());
		// normal texture for model
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, loadedModel.skinList.at(i).GetNormalMapRef());

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

		int numVertices = loadedModel.skinList.at(i).GetNumVertices();
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
	int bufferSize = loadedModel.modelNodes.GetNumBones() * 16 * 4; // every bone has a 4x4 matrix, and every value in the matrix is 4 bytes long
	glGenBuffers(1, &bonebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bonebuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, &loadedModel.modelNodes.StoreBones().at(0), GL_STATIC_DRAW);

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
	glDrawArrays(GL_POINTS, 0, loadedModel.modelNodes.GetNumBones());
	
	// Unload shader program
	glUseProgram(0);
	glDeleteBuffers(1, &bonebuffer);
	glEnable(GL_DEPTH_TEST);
}

// Function to render the main scene
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glm::quat UIConvert = -glm::quat(UIQuat[0],UIQuat[3],UIQuat[2],UIQuat[1]);

	view = glm::lookAt(
		glm::vec3(0,0, zoom),// camera position in world space
		glm::vec3(0, 0, 0), // where camera is viewing in world space
		glm::vec3(0, -1, 0)  // Y is up (in world space)
		);
	view *= glm::toMat4(UIConvert);

	// calculating MVP
	glm::mat4 MVP = projection * view * modelMatrix;

	glm::mat4 MV = view * modelMatrix;
	
	// buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Render skybox if it is toggled
	if(toggleSkybox)
	skybox.renderSkybox(view, projection);

	// Unload model shader program, pEmitter uses its own 
	glUseProgram(0);

	int timeAtStart = glutGet(GLUT_ELAPSED_TIME);

	// Wibbly wobbly timey-wimey stuff
	int delta = timeAtStart - oldTime;
	oldTime = timeAtStart;
	if(loadedModel.animCont.animationList != NULL){
		loadedModel.animCont.Update(delta);
		if (toggleSkin)
			loadedModel.Update();
	}


	if (toggleSkin) {
		// Draw skinned bones
		RenderSkin(MVP);
	}

	if (toggleBones) {
		// Draw skeleton
		RenderBones(MVP);
	}

	// Update Variables with data from ParticleEmitter 
	final_fps = pEmitter.whatIsFPS();
	particle_Count = pEmitter.particleCount() + pEmitter2.particleCount() + pEmitter3.particleCount() + pEmitter4.particleCount();

	if (toggleParticles) {
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
	
	if(multipleAnimations)
	{
		TwDefine(" GlobalOptions/NoAnimation visible=false ");
		TwDefine(" GlobalOptions/Animation-Next visible=true ");
		TwDefine(" GlobalOptions/Animation-Previous visible=true ");
	}else
	{
		TwDefine(" GlobalOptions/NoAnimation visible=true ");
		TwDefine(" GlobalOptions/Animation-Next visible=false ");
		TwDefine(" GlobalOptions/Animation-Previous visible=false ");
	}

	// Basic error handling
	glGetError();

	// menu refresh
	TwDraw();

	// OpenGL refrepsh
	glutSwapBuffers();
		
	// delete vao
	glDeleteVertexArrays(1, &vao);
	glDisable(GL_DEPTH_TEST);
}

// Mouse Controll for Zooming [a lot handier than dealing with Sticky based keys for windows
void MouseWheel(int button, int dir, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, dir, x, y))
	{
		if(button == 2){
			
		}
		// Checks if the button pressed is button 3; this is the code for positive wheel scrolling
		if (button == 3)
			zoom += 0.5f;
		// Checks if the button pressed is button 4; this is the code for negative wheel scrolling
		else if (button == 4)
			zoom -= 0.5f;

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
}

// Compiling shader code
void initShaders()
{
	ShaderLoader loader;
	particleProgram = loader.CreateProgram("ParticleVertexShader.txt", "ParticleFragmentShader.txt");
	skeletonProgram = loader.CreateProgram("NodeVertexShader.txt", "NodeFragmentShader.txt");
	redrawProgram = loader.CreateProgram("RedrawVertexShader.txt", "RedrawFragmentShader.txt");
	skyboxProgram = loader.CreateProgram("SkyboxVertexShader.VERT", "SkyboxFragmentShader.FRAG");
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
static void display() {  }
	
void CloseFunction(){
		TwTerminate();
	}

void SetNightwingEmitters()
{
	#pragma region Node Finding

	// Strings to be searched in the Node tree
	aiString leftToeBase = aiString("mixamorig_LeftToeBase");
	aiString rightToeBase = aiString("mixamorig_RightToeBase");
	aiString leftHandBase = aiString("mixamorig_LeftHand");
	aiString rightHandBase = aiString("mixamorig_RightHand");

	// Assigning a node pointer the value of a node from the node tree
	leftFoot = SearchTree(loadedModel.modelNodes.root, leftToeBase);
	rightFoot = SearchTree(loadedModel.modelNodes.root, rightToeBase);
	leftHand = SearchTree(loadedModel.modelNodes.root, leftHandBase);
	rightHand = SearchTree(loadedModel.modelNodes.root, rightHandBase);

	pEmitter.setEmitterNode(leftFoot, modelMatrix);
	pEmitter4.setEmitterNode(leftHand, modelMatrix);
	pEmitter3.setEmitterNode(rightFoot, modelMatrix);
	pEmitter2.setEmitterNode(NULL, glm::mat4(1.0f));
	#pragma endregion

	//Emitter 1 Initial Values
	#pragma region Emitter 1
	pEmitter.emitterDir[0] = 0;						// X direction
	pEmitter.emitterDir[1] = 0;						// Y Direction
	pEmitter.emitterDir[2] = 1;						// Z Direction
	pEmitter.velocity = -0.004;						// Initial velocity
	pEmitter.p_acc = glm::vec3(0);					// Accelleration
	pEmitter.p_lifetime = 50;						// Lifetime of Particles
	pEmitter.startScale = 1;						// Initial size
	pEmitter.endScale = .5;							// Size before death
	pEmitter.p_colourStart = glm::vec4(1, 0, 0, 1);	// Start colour, Red
	pEmitter.p_colourEnd = glm::vec4(0, 0, 1, 0);	// End colour, Blue
	pEmitter.angleRange = glm::vec3(0);				// Angle of emitter
	pEmitter.rate = 100;							// Rate of Spawn
	pEmitter.additive = false;						// Additive Blending
	#pragma endregion 

	//Emitter 2 Initial Values
	#pragma region Emitter 2
	pEmitter2.emitterDir[0] = 0;					// X Direction
	pEmitter2.emitterDir[1] = 1;					// Y Direction
	pEmitter2.emitterDir[2] = 0;					// Z Direction
	pEmitter2.position = glm::vec3(0,0,10);			// Position
	pEmitter2.velocity = 0.005;						// Initial Velocity
	pEmitter2.p_lifetime = 25;						// Lifetime of Particles
	pEmitter2.startScale = 10;						// Initial size
	pEmitter2.endScale = 0;							// Size before death
	pEmitter2.p_colourStart = glm::vec4(1, 0.5, 0, 1);// Start colour, 
	pEmitter2.p_colourEnd = glm::vec4(0, 0, 0, 0);	// End colour,
	pEmitter2.angleRange = glm::vec3(4);			// Angle of emitter
	pEmitter2.rate = 100;							// Rate of Spawn
	#pragma endregion 

	//Emitter 3 Initial Values
	#pragma region Emitter 3
	pEmitter3.emitterDir[0] = 0;					// X Direction
	pEmitter3.emitterDir[1] = 0;					// Y Direction
	pEmitter3.emitterDir[2] = 1;					// Z Direction
	pEmitter3.velocity = -0.004;					// Initial Velocity
	pEmitter3.p_lifetime = 50;						// Lifetime of Particles	
	pEmitter3.startScale = 1;						// Initial size
	pEmitter3.endScale = .5;						// Size before death
	pEmitter3.p_colourStart = glm::vec4(0, 1, 0, 1);// Start colour,
	pEmitter3.p_colourEnd = glm::vec4(1, 0, 0, 0);	// End colour, 
	pEmitter3.angleRange = glm::vec3(0);			// Angle of emitter
	pEmitter3.rate = 100;							// Rate of Spawn
	#pragma endregion 

	//Emitter 4 Initial Values
	#pragma region Emitter 4
	pEmitter4.emitterDir[0] = 0;					// X Direction
	pEmitter4.emitterDir[1] = 1;					// Y Direction
	pEmitter4.emitterDir[2] = 0;					// Z Direction
	pEmitter4.position = glm::vec3(0);				// Position
	pEmitter4.velocity = 0.0004;						// Initial Velocity
	pEmitter4.p_lifetime = 50;						// Lifetime of Particles
	pEmitter4.startScale = 0.5;						// Initial size
	pEmitter4.endScale = 0;							// Size before death
	pEmitter4.p_colourStart = glm::vec4(0, 0.7, 1, 1);// Start colour,
	pEmitter4.p_colourEnd = glm::vec4(0.5, 0, 0.5, 0);// End colour, 
	pEmitter4.angleRange = glm::vec3(4);			// Angle of emitter
	pEmitter4.rate = 100;							// Rate of Spawn
	#pragma endregion 
}

void SetBeastEmitters(){

	#pragma region Node Finding

	// Strings to be searched in the Node tree
	aiString mouthName = aiString("Joint13");
	aiString tailName = aiString("Joint18");

	// Assigning a node pointer the value of a node from the node tree
	aiNode *mouth = SearchTree(loadedModel.modelNodes.root, mouthName);
	aiNode *tail = SearchTree(loadedModel.modelNodes.root, tailName);

	pEmitter.setEmitterNode(mouth, modelMatrix);
	pEmitter2.setEmitterNode(tail, modelMatrix);
	#pragma endregion

	//Emitter 1 Initial Values
	#pragma region Emitter 1
	pEmitter.emitterDir[0] = 0;						// X direction
	pEmitter.emitterDir[1] = 0;						// Y Direction
	pEmitter.emitterDir[2] = -1;					// Z Direction
	pEmitter.velocity = 0;							// Initial velocity
	pEmitter.p_acc = glm::vec3(0,-0.00001,0);		// Accelleration
	pEmitter.p_lifetime = 50;						// Lifetime of Particles
	pEmitter.startScale = 0;						// Initial size
	pEmitter.endScale = .5;							// Size before death
	pEmitter.p_colourStart = glm::vec4(0, 1, 1, 1);	// Start colour
	pEmitter.p_colourEnd = glm::vec4(0, 1, 1, 0);	// End colour
	pEmitter.angleRange = glm::vec3(0);				// Angle of emitter
	pEmitter.rate = 2;								// Rate of Spawn
	pEmitter.additive = true;						// Additive Blending
	#pragma endregion 

	//Emitter 2 Initial Values
	#pragma region Emitter 2
	pEmitter2.emitterDir[0] = 0;					// X Direction
	pEmitter2.emitterDir[1] = 1;					// Y Direction
	pEmitter2.emitterDir[2] = 0;					// Z Direction
	pEmitter2.velocity = -0.001;					// Initial Velocity
	pEmitter2.p_lifetime = 15;						// Lifetime of Particles
	pEmitter2.startScale = 0.7;						// Initial size
	pEmitter2.endScale = 0;							// Size before death
	pEmitter2.p_colourStart = glm::vec4(0, 1, 0, 1);// Start colour, 
	pEmitter2.p_colourEnd = glm::vec4(0, 1, 1, 0);	// End colour,
	pEmitter2.angleRange = glm::vec3(4);			// Angle of emitter
	pEmitter2.rate = 100;							// Rate of Spawn
	#pragma endregion 
}

void DisableEmitters(){
	pEmitter.rate = 0;
	pEmitter2.rate = 0;
	pEmitter3.rate = 0;
	pEmitter4.rate = 0;
}

void loadModel(int modelNo){

		#pragma region Set Default Values
		modelMatrix = glm::mat4(1.0f);
		pEmitter.setEmitterNode(NULL, modelMatrix);
		pEmitter2.setEmitterNode(NULL, modelMatrix);
		pEmitter3.setEmitterNode(NULL, modelMatrix);
		pEmitter4.setEmitterNode(NULL, modelMatrix);
		modelMatrix = glm::translate(glm::vec3(0,-2,0));
		DisableEmitters();
		#pragma endregion

		switch(modelNo){
		// Iron Man (Static Model)
		case 0:
				#pragma region Iron Man
				loadedModel.LoadModelData("Models/IronMan/Iron_Man.dae");
				modelMatrix *= glm::rotate(modelMatrix,3.1f,glm::vec3(0,1,0));
				modelMatrix *= glm::translate(glm::vec3(0,2,0));
				multipleAnimations = false;
				loadedModel.Update();
				#pragma endregion
		break;
		// Zombie
		case 1:
				#pragma region Zombie
				loadedModel.LoadModelData("Models/Zombie/Zombie_Idle02_roar.X");
				loadedModel.animCont.animationSpeed = 200;
				animationSplit("none");
				loadedModel.animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);
				modelMatrix *= glm::rotate(modelMatrix,3.1f,glm::vec3(0,1,0));
				modelMatrix *= glm::scale(glm::vec3(0.1,0.1,0.1));
				modelMatrix *= glm::translate(glm::vec3(0,20,0));
				#pragma endregion
		break;
		// Ninja
		case 2:
				#pragma region Ninja
				loadedModel.LoadModelData("Models/Ninja/ninjaEdit.ms3d");
				loadedModel.animCont.animationSpeed = 1;
				animationSplit("ninja");
				loadedModel.animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);
				
				modelMatrix *= glm::scale(glm::vec3(0.5,0.5,0.5));
				#pragma endregion
		break;
		// Beast
		case 3:
				#pragma region Beast
				loadedModel.LoadModelData("Models/Beast/beastedit.ms3d");
				loadedModel.animCont.animationSpeed = 1;
				animationSplit("beast");
				loadedModel.animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);
				modelMatrix *= glm::rotate(modelMatrix,3.1f,glm::vec3(0,1,0));
				modelMatrix *= glm::scale(glm::vec3(0.05,0.05,0.05));
				modelMatrix *= glm::translate(glm::vec3(0,50,-6));
				SetBeastEmitters();
				#pragma endregion
		break;
		// Nightwing
		case 4:
				#pragma region Nightwing
				loadedModel.LoadModelData("Models/NightWingAS/nightwing anim.dae");
				loadedModel.animCont.animationSpeed = 0.05;
				animationSplit("none");
				loadedModel.animCont.SetLoopTime(animationTimes.at(0).x,animationTimes.at(0).y);

				modelMatrix = glm::rotate(modelMatrix,3.1f,glm::vec3(0,1,0));
				modelMatrix = glm::rotate(modelMatrix,1.55f,glm::vec3(1,0,0));
				SetNightwingEmitters();
				#pragma endregion
		break;
		}
	}

#pragma region Emitter Button Functions

	// Change Emitter's graphic to Smoke
	void TW_CALL emitSmoke(void *clientData)
	{ 
		pEmitter.StoreParticleTextureData("Models/SmokeShape.png");
	}

	// Change Emitter's graphic to Phil
	void TW_CALL emitPhil(void *clientData)
	{ 
		pEmitter.StoreParticleTextureData("Models/p.hanna.jpg");
	}

	// Change Emitter's graphic to a Spark
	void TW_CALL emitSpark(void *clientData)
	{ 
		pEmitter.StoreParticleTextureData("Models/Spark.png");
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
	void TW_CALL prevAnim(void *clientData)
	{
		if (currentAnimation > 0)
			currentAnimation--;
		else
			currentAnimation = animationTimes.size() - 1;
		loadedModel.animCont.SetLoopTime(animationTimes.at(currentAnimation).x, animationTimes.at(currentAnimation).y);
	}

	// Call that goes to the previous stage in the animation
	void TW_CALL nextAnim(void *clientData)
	{
		if (currentAnimation < animationTimes.size() - 1)
			currentAnimation++;
		else
			currentAnimation = 0;
		loadedModel.animCont.SetLoopTime(animationTimes.at(currentAnimation).x, animationTimes.at(currentAnimation).y);
	}

	void TW_CALL nextModel(void* clientData)
	{
		if(modelCycle < 4)
				modelCycle++;
			else
				modelCycle = 0;

			loadModel(modelCycle);
	}

	void TW_CALL prevModel(void* clientData)
	{
		if(modelCycle > 0)
				modelCycle--;
			else
				modelCycle = 4;

			loadModel(modelCycle);
	}
	
	void TW_CALL ListNodes(void* clientData)
	{
		loadedModel.modelNodes.PreOrderTraversal();
	}
	
	void TW_CALL resetCamera(void* clientData)
	{
		zoom = 10;
		UIQuat[0] = 0;
		UIQuat[1] = 0;
		UIQuat[2] = 0;
		UIQuat[3] = 1;
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
	
	// Set the clear colour to blue
	glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
	
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

	skybox.loadSkybox(skyboxProgram, "Textures/Nissi/",
						"posz.jpg",
						"negz.jpg",
						"posx.jpg",
						"negx.jpg",
						"posy.jpg",
						"negy.jpg");

	pEmitter.StoreParticleTextureData("Models/SmokeShape.png");
	
	#pragma region Emitter Creation

	//Create a particle emitter
	pEmitter = ParticleEmitter(particleProgram,	// Shader
		leftFoot,								// Paired Node
		"Models/SmokeShape.png");				// Texture

	pEmitter2 = ParticleEmitter(particleProgram,	// Shader
		leftHand,									// Paired Node
		"Models/SmokeShape.png");					// Texture

	pEmitter3 = ParticleEmitter(particleProgram,	// Shader
		rightFoot,									// Paired Node
		"Models/SmokeShape.png");					// Texture

	pEmitter4 = ParticleEmitter(particleProgram,	// Shader
		NULL,										// No paired node
		"Models/SmokeShape.png");					// Texture

	#pragma endregion

	// loading model from file
	loadModel(0);

	#pragma region TweakBar

	#pragma region Global Options Bar
	tBar = TwNewBar("GlobalOptions");
	TwDefine("GlobalOptions size='200 500'" /*color='96 216 224' "*/);
	TwDefine("GlobalOptions position='20 20' ");

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
	TwAddButton(tBar, "Emit Spark", emitSpark, NULL, " label='Emit Spark'");
	TwAddSeparator(tBar, NULL, " ");

	TwAddVarRW(tBar, "Toggle Particle Emitter", TW_TYPE_BOOLCPP, &toggleParticles, " ");
	TwAddVarRW(tBar, "Toggle Skin", TW_TYPE_BOOLCPP, &toggleSkin, " ");
	TwAddVarRW(tBar, "Toggle Bones", TW_TYPE_BOOLCPP, &toggleBones, " ");
	TwAddVarRW(tBar, "Toggle Skybox", TW_TYPE_BOOLCPP, &toggleSkybox, " ");
	TwAddSeparator(tBar, NULL, " ");

	TwAddButton(tBar, "Next Model", nextModel, NULL, " ");
	TwAddButton(tBar, "Previous Model", prevModel, NULL, " ");
	TwAddSeparator(tBar, NULL, " ");

	// Animation control buttons
	TwAddButton(tBar, "NoAnimation", NULL, NULL, " label='No Animation Switching' ");
	TwAddButton(tBar, "Animation-Next", nextAnim, NULL, "visible=false help='Only use when Ninja or Beast is active' ");
	TwAddButton(tBar, "Animation-Previous", prevAnim, NULL, "visible=false help='Only use when Ninja or Beast is active' ");
	TwAddSeparator(tBar, NULL, " ");

	TwAddButton(tBar, "List Model Nodes", ListNodes, NULL, "visible=true help='Lists the nodes of a model, their depth in the tree and their transformations' ");
	#pragma endregion
		
	#pragma region Camera Options Bar
	
	TwBar *camBar = TwNewBar("CameraOptions");
	TwDefine("CameraOptions size='200 150'");
	TwDefine("CameraOptions position='300 650' ");
	TwAddVarRW(camBar, "Camera Angle", TW_TYPE_QUAT4F, &UIQuat, "opened=true ");
	TwAddVarRW(camBar, "Camera Distance", TW_TYPE_FLOAT, &zoom, " ");
	TwAddButton(camBar, "Reset Camera", resetCamera, NULL, " ");
	#pragma endregion

	// Emitter GUI Setup
	
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
	glutCloseFunc(CloseFunction);
	glutMouseFunc(MouseWheel);

	glutMainLoop();
}
