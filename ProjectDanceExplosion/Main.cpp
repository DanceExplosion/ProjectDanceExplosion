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

// Inner Project Classes
#include "ShaderLoader.h"
#include "Mesh.h"

// OpenGl Essentials
GLuint window;
GLuint basicProgram;

// Camera
glm::mat4 projection;
glm::mat4 view;
int xCircle = 1, yCircle = 1;
float camX = 0, camY = 0, zoom = -4;//-800;

// Models
std::vector<Mesh> modelList;
int numModels;

void LoadModelData()
{
	// set background colour //black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Assimp file importer
	Assimp::Importer importer;

	std::string fileRoot = "Models/";
	//std::string file = fileRoot + "Nightwing187/Nightwing187.dae";
	std::string file = fileRoot + "IronMan/Iron_Man.dae";
	//std::string file = fileRoot + "Optimus/Optimus.dae";
	//std::string file = fileRoot + "Robin188/Robin188.dae";
	//std::string file = fileRoot + "GreenArrow/GreenArrow.dae";
	//std::string file = fileRoot + "Bear_Brown/Bear_Brown.dae";
	//std::string file = fileRoot + "C3P0/C3P0.dae";

	const char* filePath = file.c_str();

	const aiScene* scene = importer.ReadFile(filePath,
								aiProcess_CalcTangentSpace |
								aiProcess_Triangulate |
								aiProcess_JoinIdenticalVertices |
								aiProcess_SortByPType |
								aiProcess_GenNormals |
								aiProcess_GenUVCoords |
								aiProcess_SortByPType
								);

	// error checking
	if(!scene)
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "File loaded successfully" <<std::endl;
	
	int anim = scene->mNumAnimations;
	numModels = (scene->mNumMeshes);
	int numTextures = scene->mNumMaterials;
	
	std::cout << "Number of models in file: " << numModels << std::endl;
	std::cout << "Number of external textures in file: " << numTextures << std::endl;
	std::cout << "Number of embedded textures in file: " << scene->mNumTextures << std::endl;
	std::cout << ""<< std::endl;

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

	// model's model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	// calculating MVP
	glm::mat4 MVP = projection * view * modelMatrix;

	// set shader program
	glUseProgram(basicProgram);

	// Buffering & linking variables for Shaders
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//bind BasicVertexShader.MVP to this.matrixId
	GLuint matrixId = glGetUniformLocation(basicProgram, "MVP");
	
	// USING SHADERS
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);
	

	for(int i = 0; i < numModels; i++)
	{
		// vertex buffer		
		int bufferSize = modelList.at(i).GetNumVertices() * 3 * 4; // each vertex has 3 parts(x, y & z), each part is 4 bytes long
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, modelList.at(i).GetVertexData(), GL_STATIC_DRAW);

		// Texturing
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

		// find correct texture for model
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, modelList.at(i).GetTextureData());

		// bind fragmentShader.textureSampler to this.textureSampler
		GLuint textureSampler = glGetUniformLocation(basicProgram, "textureSampler");
		// pass in texture data
		glUniform1i(textureSampler, 0);

		// pass vertex data
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
			GL_TRUE,		// normalised?
			0,				// stride
			(void*)0		// array buffer offset
			);

		// draw model
		int numVertices = modelList.at(i).GetNumVertices();
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		// TEXTURING
		glBindTexture(GL_TEXTURE_2D, NULL);
		glActiveTexture(NULL);

		// disable & delete vbo, texturebuffer & normalbuffer
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &texturebuffer);
		glDeleteBuffers(1, &normalbuffer);
	}

	glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
		
	// delete vao
	glDeleteBuffers(1, &vao);
}

// Calculate view matrix
void MoveCamera(/*int x, int y*/)
{
	/*
	float camX, camZ;

	// change cameraRadius if model cannot be seen
	float cameraRadius = 120.0f;

	xCircle += x + y;
	yCircle += y;
	if( y == 0 )// move in X and Z axis
	{
		camX = cameraRadius * cos(xCircle * 3.14f / 180.0f);
		camZ = (cameraRadius * sin(xCircle * 3.14f / 180.0f))+zoom;
	}
	else // move in X, Y and Z axis
	{
		camX = cameraRadius * cos(xCircle * 3.14f / 180.0f);
		camY = cameraRadius * sin(yCircle * 3.14f / 180.0f);
		camZ = (120.0f * sin(xCircle * 3.14f / 180.0f))+zoom;
	}

	if ((xCircle > 360.0f) || (xCircle < -360.0f))
		xCircle %= 360;

	// camera position in world space
	glm::vec3 pos = glm::vec3(camX , camY, zoom);
	*/
	view = glm::lookAt(
		glm::vec3(6, 0, zoom),// camera position in world space
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
	basicProgram = loader.CreateProgram("BasicVertexShader.txt", "BasicFragmentShader.txt");
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
	
	// loading shaders
	initShaders();
	// setting up MVP
	initCamera();
	// loading models from file
	LoadModelData();

	//
	glutDisplayFunc(RenderScene);

	// keyboard control
	glutKeyboardFunc(KeyPress);
	glutSpecialFunc(CameraControls);

	glutMainLoop();
}