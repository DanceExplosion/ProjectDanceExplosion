//Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenGL Mathemathics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Standard Vector Library
#include <vector>

// Header File
#include "ParticleEmitter.h"


// Creation of Particle Structure
typedef struct Particle{

	// Particle Variables
	#pragma region Variables

	// Variables for the structure
	float lifetime;				// Particle Lifetime
	glm::vec4 colour;			// Particle Colour
	glm::vec3 velocity;			// Particle Speed Vector
	glm::vec3 acc;				// Acceleration Vector
	glm::vec3 position;			// Particle Position
	char r,g,b,a;				// Particle Colour 2		<--- This may be removed, or the vec4 will be removed, need to decide which to keep
	float cameradistance;		// Distance from the camera. set to -1 when dead. Used for sorting
	#pragma endregion

	// Set the meaning of the < comparison for two particles
	bool operator<(const Particle& that) const 
	{
		// Sort in reverse order, 
		return this->cameradistance > that.cameradistance;
	}

	// Initializing constructor
	Particle() {
		// Constructor: sets variables to zero
		position = glm::vec3(0,0,0);
		velocity = glm::vec3(0,0,0);
		acc = glm::vec3(0,0,0);
		lifetime = 0;
	};
	
	// Functions for the Particles
	#pragma region Particle Functions

	// Update particles
	void update(float delta){
		
			// gotta take away yo life fool
			// we dont know what this value is lol
			lifetime -= delta*0.16f;

			// Gotta go fast
			// Update Velocity based off acceleration
			// Update Position based off velocity 
			velocity += acc;
			position += velocity;
	};

	// Checks if a particle is dead
	bool isDead(){
		return lifetime <= 0;
	};

	// Function to return the lifetime
	float getLifeTime(){
		return lifetime;
	};

	// Function to return the position of a particle
	glm::vec3 getPosition(){
		return position;
	};
	#pragma endregion
}Particle; // end of Particle struct

// Global 
#pragma region Variables

// Shader ID
GLuint programID;

// Annoying
const static int MAXPARTICLES = 10000;

// Gotta get that time yo
int oldTime = 0;

// Variables for Buffers
GLuint billboard_vertex_buffer, particles_position_buffer, particles_colour_buffer;
static const GLfloat g_vertex_buffer_data[] = {
	-0.1f, -0.1f, 0,
	0.1f, -0.1f, 0,
	-0.1f, 0.1f, 0,
	0.1f, 0.1f, 0,
};

// Position and Colour data arrays for loading into buffers
static GLfloat* g_particule_position_data = new GLfloat[MAXPARTICLES * 3];
static GLubyte* g_particule_colour_data = new GLubyte[MAXPARTICLES * 4];

// Container For Particles
Particle ParticleContainer[MAXPARTICLES];

// The index of the last particle retrieved from the array
int LastUsedParticle = 0;

// The amount of particles to be drawn
int ParticlesCount = 0;
#pragma endregion

// Finds a Particle in ParticlesContainer which is dead.
int FindUnusedParticle(){
	// Loop through all particles, starting at the location of the last dead particle
	for (int i = LastUsedParticle; i<MAXPARTICLES; i++){
		// If the particle is dead, note it's index and return it
		if (ParticleContainer[i].lifetime <= 0){
			LastUsedParticle = i;
			return i;
		}
	}
	// If all particles are used, overwrite the first particle
	LastUsedParticle = 0;
	return 0;
}

void SortParticles(){
	std::sort(&ParticleContainer[0], &ParticleContainer[MAXPARTICLES]);
}

// Blank constructor
ParticleEmitter::ParticleEmitter(){}

// Main constructor for a Particle Emitter
// Passes through position, velocity, acceleration, lifeTime and colour
ParticleEmitter::ParticleEmitter(GLuint shaderProgram, glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col){

	// Assignment of parameters
	programID = shaderProgram;
	position = pos;
	p_velocity = vel;
	p_acc = accel;
	p_lifetime = life;
	p_colour = col;

	// Loop to initialize all particles
	for (int i = 0; i < MAXPARTICLES; i++)
	{
		ParticleContainer[i] = Particle();
	}

	// Use the Shader Program
	glUseProgram(programID);

	// Bind the buffers to their respective arrays
	#pragma region Binding of Buffers

	//Buffers for Drawing Particles
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colours of the particles
	glGenBuffers(1, &particles_colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_colour_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

#pragma endregion

}

// Main Update Loop
void ParticleEmitter::PEmitterUpdate(){
	
	// Update Function for the Particle Emitter

	// Use ShaderProgram
	glUseProgram(programID);

	// Wibbly wobbly timey wimey stuff
	int timeAtStart = glutGet(GLUT_ELAPSED_TIME);
	int delta = timeAtStart - oldTime;
	oldTime = timeAtStart;

#pragma region Particle Updating

	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta);
	if (newparticles > (int)(0.016f*100.0))
		newparticles = (int)(0.016f*100.0);

	// Loops through every new particle
	for (int i = 0; i < newparticles; i++){
		
		// Sets the Index to the value returned from the FindUsedParticle() function
		// Goes throug the ParticleContainer and sets the necessary variables
		int particleIndex = FindUnusedParticle();
		ParticleContainer[particleIndex].lifetime = p_lifetime;
		ParticleContainer[particleIndex].position = position;
		ParticleContainer[particleIndex].velocity = p_velocity*glm::vec3(
															(rand()%2000 - 1000.0f)/1000.0f,
															(rand()%2000 - 1000.0f)/1000.0f,
															(rand()%2000 - 1000.0f)/1000.0f);
		ParticleContainer[particleIndex].acc = p_acc;

		// Generates a random colour for each particle
		// RGB and A
			ParticleContainer[particleIndex].r = rand() % 256;
			ParticleContainer[particleIndex].g = rand() % 256;
			ParticleContainer[particleIndex].b = rand() % 256;
			ParticleContainer[particleIndex].a = (rand() % 256) / 3;
	}

	// Check for all Particles
	ParticlesCount = 0;
	for (int i = 0; i < MAXPARTICLES; i++){

		// Create a pointer to the ParticleContainer at position i
		Particle& p = ParticleContainer[i];
		
		// Check that the current particle isnt dead
			if (!p.isDead()){
				// Run the update function for that particle
				p.update(delta);
				// Check again that the particle is not dead
				if (!p.isDead()){

					// Set p's cameradistance to its lifetime
					p.cameradistance = p.lifetime;
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					// Positional data for each particle
					g_particule_position_data[3 * ParticlesCount + 0] = p.position.x;
					g_particule_position_data[3 * ParticlesCount + 1] = p.position.y;
					g_particule_position_data[3 * ParticlesCount + 2] = p.position.z;
					// Colour data for each particle
					g_particule_colour_data[4 * ParticlesCount + 0] = p.r;
					g_particule_colour_data[4 * ParticlesCount + 1] = p.g;
					g_particule_colour_data[4 * ParticlesCount + 2] = p.b;
					g_particule_colour_data[4 * ParticlesCount + 3] = p.a;

					// Increment ParticleCount
				ParticlesCount++;

				}
				else{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}
			}
	}
	// Sort the dead particles from the alive particles
	SortParticles();
#pragma endregion

}

void ParticleEmitter::PEmitterDraw(glm::mat4 viewMatrix, glm::mat4 vp_maxtrix){

	// Draw Function for the Particle Emitter
	glm::vec3 CameraRight_worldspace = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glm::vec3  CameraUp_worldspace = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
#pragma region Drawing of Buffers

	// Checks ParticlesCount is greater than 0
	if(ParticlesCount > 0){

	#pragma region Buffer Streaming 
	
	// Initialises shader program
	glUseProgram(programID);

	// Creation of VBO for the vertices of the particles
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 3, g_particule_position_data);
	
	// Creation of the VBO for colour of the particles
	glGenBuffers(1, &particles_colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_colour_data);
	#pragma endregion

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Vertex shader
	GLuint VP_Matrix_ID = glGetUniformLocation(programID, "VP");
	GLuint CamRight_ID = glGetUniformLocation(programID, "CamRight_W");
	GLuint CamUp_ID = glGetUniformLocation(programID, "CamUp_W");

	glUniformMatrix4fv(VP_Matrix_ID, 1, GL_FALSE, &vp_maxtrix[0][0]);
	glUniform3f(CamRight_ID, CameraRight_worldspace.x,CameraRight_worldspace.y,CameraRight_worldspace.z);
	glUniform3f(CamUp_ID, CameraUp_worldspace.x,CameraUp_worldspace.y,CameraUp_worldspace.z);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0, // attribute. No particular reason for 0, but must match the layout in the shader.
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but must match the layout in the shader.
		3, // size : x + y + z => 3
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);

	// 3rd attribute buffer : particles' colours
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_colour_buffer);
	glVertexAttribPointer(
		2, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
		);
		
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1); // colour : one per quad -> 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	// Partial Cleanup, Disabling of VertexAttribArrays
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	}
#pragma endregion

	// Unload particle shader program
	glUseProgram(0);
}

void ParticleEmitter::PEMitterCleanup(){

	// CleanUp function

	// Cleanup VBO 
	glDeleteBuffers(1, &particles_colour_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);

	glDisable(GL_BLEND);
}