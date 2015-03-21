//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenGL Mathemathics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

// Standard Vector Library
#include <vector>
#include <iostream>

//Particle Emitter Class
class ParticleEmitter
{
public:
	//Variables for the Emitter to modify
	float p_lifetime;
	glm::vec4 p_colour;
	glm::vec3 p_velocity;
	glm::vec3 p_acc;
	glm::vec3 position;
	

	//Value for max number of Particles in the system
	//10 is hardcore
	//const static int MaxParticles = 10;
	
	// Default Constructor
	ParticleEmitter();

	// ParticleEmitter function, passes in the variables from above
	ParticleEmitter(GLuint shaderProgram, glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col);
	
	// ParticleEmitterUpdate function
	void Update(float delta);

	// Draw Function
	void Draw(glm::mat4 viewMatrix, glm::mat4 vpmatrix);

	// CleanUp of Arrays
	void Cleanup();

	// Function for getting Texture for Particles
	void StoreParticleTextureData();

	// Function for printing out the current particle count
	void particleCount();

	// Function to disable emitter (doesnt work)
	void disable();

	// Gets the FPS of the particle emitter
	void whatIsFPS();

	GLuint textureRef;

};