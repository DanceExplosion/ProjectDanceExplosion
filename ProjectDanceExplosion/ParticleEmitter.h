#pragma once
//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenGL Mathemathics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Standard Vector Library
#include <vector>

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
	
	ParticleEmitter();

	//ParticleEmitter function, passes in the variables from above
	ParticleEmitter(GLuint shaderProgram, glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col);
	
	//ParticleEmitterUpdate function
	void PEmitterUpdate();

	//Draw Function
	void PEmitterDraw(glm::mat4 viewMatrix, glm::mat4 vpmatrix);

	//CleanUp of Arrays
	void PEMitterCleanup();

	void particleShaders();

};