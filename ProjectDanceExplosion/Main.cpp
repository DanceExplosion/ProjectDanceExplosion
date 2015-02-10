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
float camY;

// Models
std::vector<Mesh> modelList;
int numModels;

void LoadModelData()
{
	// set background colour //black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Assimp file importer
	Assimp::Importer importer;

	std::string file = "teapot.obj";
	const char* filePath = file.c_str();

	const aiScene* scene = importer.ReadFile(filePath,
								aiProcess_CalcTangentSpace |
								aiProcess_Triangulate |
								aiProcess_JoinIdenticalVertices |
								aiProcess_SortByPType
								);
	// error checking
	if(!scene)
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
	else
		std::cout << "File loaded successfully" <<std::endl;

	numModels = scene->mNumMeshes;
	std::cout << "Number of models in file: " << numModels << std::endl;

	// Pulling required data from scene
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh1 = scene->mMeshes[i];
		modelList.push_back(Mesh(mesh1));
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

		int numVertices = modelList.at(i).GetNumVertices();

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

		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		// disable & delete vbo
		glDisableVertexAttribArray(0);
		glDeleteBuffers(1, &vbo);
	}

	glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
		
	// delete vao & vbo
	glDeleteBuffers(1, &vao);
}

// Calculate view matrix
void MoveCamera(int x, int y)
{
	float camX, camZ;

	// change cameraRadius if model cannot be seen
	float cameraRadius = 120.0f;

	xCircle += x + y;
	yCircle += y;
	if( y == 0 )// move in X and Z axis
	{
		camX = cameraRadius * cos(xCircle * 3.14f / 180.0f);
		camZ = cameraRadius * sin(xCircle * 3.14f / 180.0f);
	}
	else // move in X, Y and Z axis
	{
		camX = cameraRadius * cos(xCircle * 3.14f / 180.0f);
		camY = cameraRadius * sin(yCircle * 3.14f / 180.0f);
		camZ = 120.0f * sin(xCircle * 3.14f / 180.0f);
	}

	if ((xCircle > 360.0f) || (xCircle < -360.0f))
		xCircle %= 360;

	// camera position in world space
	glm::vec3 pos = glm::vec3(camX , camY, camZ);
	
	view = glm::lookAt(
		pos,				// camera position in world space
		glm::vec3(0, 0, 0), // where camera is viewing in world space
		glm::vec3(0, 1, 0)  // Y is up (in world space)
		);

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
		MoveCamera(0, 5);
		break;
	case GLUT_KEY_DOWN:
		MoveCamera(0,-5);
		break;
	case GLUT_KEY_RIGHT:
		MoveCamera(-5,0);
		break;
	case GLUT_KEY_LEFT:
		MoveCamera(5,0);
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
	MoveCamera(0,0);
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
	glutInitWindowSize(800, 800); // window siz3e
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
	// loading first model of file
	LoadModelData();

	//
	glutDisplayFunc(RenderScene);

	// keyboard control
	glutKeyboardFunc(KeyPress);
	glutSpecialFunc(CameraControls);

	glutMainLoop();
}