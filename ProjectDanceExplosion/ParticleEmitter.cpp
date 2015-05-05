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



// Global 
#pragma region Variables

// Blank initialization for d_Store and fps
float d_Store = 0.0f;
float fps = 0.0f;

// Shader ID
GLuint programID;

bool additive;


	// Annoying
	const static int MAXPARTICLES = 10000;

	// Container For Particles
	Particle ParticleContainer[MAXPARTICLES];

// Variables for Buffers
GLuint billboard_vertex_buffer, particles_position_buffer, particles_colour_buffer, particle_texture_buffer;
GLfloat g_vertex_buffer_data[] = {
	-0.7, -0.7, 0,
	0.7, -0.7, 0,
	-0.7, 0.7, 0,
	0.7, 0.7, 0,
};

// Array to hold UV's for particle textures
static const GLfloat g_texture_buffer_data[] = {
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};


// Position and Colour data arrays for loading into buffers
static GLfloat* g_particle_position_data = new GLfloat[MAXPARTICLES * 4];
static GLubyte* g_particle_colour_data = new GLubyte[MAXPARTICLES * 4];


// The index of the last particle retrieved from the array


// The amount of particles to be drawn
int ParticlesCount = 0;
#pragma endregion


// Get a randomly generated value between 1 and 0
float getRand(){
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// Blank constructor
ParticleEmitter::ParticleEmitter(){}


	int LastUsedParticle;

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

// Main constructor for a Particle Emitter
// Passes through position, velocity, acceleration, lifeTime and colour
ParticleEmitter::ParticleEmitter(GLuint shaderProgram, aiNode* node, GLuint texture){

	// Assignment of parameters
	programID = shaderProgram;
	position = glm::vec3(0);
	p_acc = glm::vec3(0);
	p_lifetime = 20;
	velocity = 0.006;
	velocityRange = 0;
	p_colourStart = glm::vec4(1);
	p_colourEnd = glm::vec4(0,0,0,0);
	startScale = 1;
	endScale = 0.2;
	rate = 10;
	rateCounter = 0;
	angleRange = glm::vec3(0.5,0.5,0.5);
	emitterDir[0] = 1;
	emitterDir[1] = 0;
	emitterDir[2] = 0;
	LastUsedParticle = 0;

	textureRef = texture;

	sPos_Node = node;

	additive = false;

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

	//Buffers for Drawing Particles
	glGenBuffers(1, &particle_texture_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particle_texture_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_buffer_data), g_texture_buffer_data, GL_STATIC_DRAW);

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

	// End the shader
	glUseProgram(0);

#pragma endregion

#pragma region Particle Texture

	// Function call for getting the Texture for the particle

#pragma endregion

}

/*
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

	//Buffers for Drawing Particles
	glGenBuffers(1, &particle_texture_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particle_texture_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_buffer_data), g_texture_buffer_data, GL_STATIC_DRAW);

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

	// End the shader
	glUseProgram(0);

#pragma endregion

#pragma region Particle Texture

	// Function call for getting the Texture for the particle
	StoreParticleTextureData();

#pragma endregion

}
*/


// Main Update Loop
void ParticleEmitter::Update(float delta, glm::mat4 view){
	
	// Update Function for the Particle Emitter

	// Use ShaderProgram
	glUseProgram(programID);

	// Value store for FPS calculations later
	d_Store = delta;

#pragma region Particle Updating

	// Generate 10 new particle each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = 0;
	// Increment the counter by the rate, with respect to time
		rateCounter += delta*rate/100;

		// If the rate reaches the threshold, create new particles
		if(rateCounter >= 10){
			// If the rate is high enough, then the rate counter could end up several times higher than 10, so create as many particles as appropriate
			newparticles = rateCounter/10;

			// Reduce the rate counter to a value lower than 10, but bigger than 0 (in the event of newparticles rounding up)
			rateCounter = rateCounter-(newparticles*10);
			if(rateCounter <= 0)
				rateCounter = 0;
		}

	//newparticles = 1;

#pragma region Node Positions for Emitters
		glm::vec3 startPos;

		if(sPos_Node != NULL){
	aiNode* tempNode = sPos_Node;
	aiMatrix4x4 sPos = sPos_Node->mTransformation;

	// Get the total transformation of the node
	while (tempNode->mParent != NULL)
	{
		aiMatrix4x4 temp = tempNode->mParent->mTransformation;
		sPos = temp*sPos;
		tempNode = tempNode->mParent;
	}

	sPos = sPos.Transpose();

	glm::mat4 sPos_Converted = glm::mat4(
		sPos.a1, sPos.a2, sPos.a3, sPos.a4,
		sPos.b1, sPos.b2, sPos.b3, sPos.b4,
		sPos.c1, sPos.c2, sPos.c3, sPos.c4,
		sPos.d1, sPos.d2, sPos.d3, sPos.d4);

	glm::vec4 temp = sPos_Converted * glm::vec4(position, 1);

	startPos = glm::vec3(temp.x, temp.y, temp.z);
		}else{
			startPos = position;
		}

#pragma endregion
		
		// Get the vector position of the camera
		glm::vec3 CameraPosition(glm::inverse(view)[3]);

	// Loops through every new particle
	for (int i = 0; i < newparticles; i++){

		// Sets the Index to the value returned from the FindUsedParticle() function
		// Goes throug the ParticleContainer and sets the necessary variables
		int particleIndex = FindUnusedParticle();
		

		if (particleIndex != -1)
		{
			// Generate a random velocity based on the velocity range
			float currentVel = velocity+ (velocityRange*getRand() - velocityRange*getRand());

			// Generate random angles for the X, Y and Z directions
			float xAngle = emitterDir[0]+(angleRange.x*getRand() - angleRange.x*getRand());
			float yAngle = emitterDir[1]+(angleRange.y*getRand() - angleRange.y*getRand());
			float zAngle = emitterDir[2]+(angleRange.z*getRand() - angleRange.z*getRand());

			// Normalize the angles to make the speed of each particle the same
			glm::vec3 angle = glm::normalize(glm::vec3(xAngle,yAngle,zAngle));

			// Set the speed of the particle to the randomly generated values
			ParticleContainer[particleIndex].velocity.x = -angle.x*currentVel;
			ParticleContainer[particleIndex].velocity.y = angle.y*currentVel;
			ParticleContainer[particleIndex].velocity.z = angle.z*currentVel;

			// Checks the Index of the current particle isnt out of bounds
			// Sets the values of each particle to the parameter values from the Emitter initialisation
			ParticleContainer[particleIndex].lifetime = p_lifetime;
			ParticleContainer[particleIndex].startLifetime = p_lifetime;
			ParticleContainer[particleIndex].position = startPos;
			//ParticleContainer[particleIndex].velocity = p_velocity;
			ParticleContainer[particleIndex].acc = p_acc;
			
			// Set the RGBA values of Particles in the container to the passed in values 
			ParticleContainer[particleIndex].currentColour = glm::vec4(0);
			ParticleContainer[particleIndex].startColour = p_colourStart;
				ParticleContainer[particleIndex].endColour = p_colourEnd;

			// Store the starting, current and ending values of the particle's scale
			ParticleContainer[particleIndex].startSize = startScale;
			ParticleContainer[particleIndex].endSize = endScale;
			ParticleContainer[particleIndex].currentSize = startScale;

			// Calculate it's distance from the camera
			ParticleContainer[particleIndex].cameradistance = glm::length2(position - CameraPosition);;
		}

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
				// Set p's cameradistance to its lifetime, unless it has just been created, in which case, make sure it isn't drawn
				// This avoids a glitch that caused newly created particles to always be drawn in front
				if(p.lifetime < p.startLifetime - 0.016f*delta)
				p.cameradistance = glm::length2(p.position - CameraPosition);
				else
					p.cameradistance = INFINITE;
				//p.cameradistance = p.lifetime;
				
				// Fill the GPU buffer
				// Positional data and size for each particle (xyzw)
				if(p.cameradistance != INFINITE){
				g_particle_position_data[4 * ParticlesCount + 0] = p.position.x;
				g_particle_position_data[4 * ParticlesCount + 1] = p.position.y;
				g_particle_position_data[4 * ParticlesCount + 2] = p.position.z;
				g_particle_position_data[4 * ParticlesCount + 3] = p.currentSize;

				// Colour data for each particle (rgba)
				g_particle_colour_data[4 * ParticlesCount + 0] = p.currentColour.r*255;
				g_particle_colour_data[4 * ParticlesCount + 1] = p.currentColour.g*255;
				g_particle_colour_data[4 * ParticlesCount + 2] = p.currentColour.b*255;
				g_particle_colour_data[4 * ParticlesCount + 3] = p.currentColour.a*255;

				// Increment ParticleCount
				ParticlesCount++;
				}else{
				g_particle_colour_data[4 * ParticlesCount + 0] = 0;
				g_particle_colour_data[4 * ParticlesCount + 1] = 0;
				g_particle_colour_data[4 * ParticlesCount + 2] = 0;
				g_particle_colour_data[4 * ParticlesCount + 3] = 0;
				}
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

// Draw Function for the Particle Emitter
void ParticleEmitter::Draw(glm::mat4 viewMatrix, glm::mat4 vp_maxtrix){

	// Matrix variables 
	glm::vec3 CameraRight_worldspace = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glm::vec3  CameraUp_worldspace = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

#pragma region Drawing of Buffers

	// Checks ParticlesCount is greater than 0
	if (ParticlesCount > 0){

#pragma region Buffer Streaming 

		// Initialises shader program
		glUseProgram(programID);

		// Creation of VBO for the vertices of the particles
		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// Creation of VBO for the vertices of the particles
		glGenBuffers(1, &particle_texture_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particle_texture_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_buffer_data), g_texture_buffer_data, GL_STATIC_DRAW);

		// Update the buffers that OpenGL uses for rendering.
		// There are much more sophisticated means to stream data from the CPU to the GPU, 
		// but this is outside the scope of this tutorial.
		// http://www.opengl.org/wiki/Buffer_Object_Streaming
		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat)* 4, g_particle_position_data);

		// Creation of the VBO for colour of the particles
		glGenBuffers(1, &particles_colour_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_colour_buffer);
		glBufferData(GL_ARRAY_BUFFER, MAXPARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte)* 4, g_particle_colour_data);
#pragma endregion

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glEnable(GL_CULL_FACE);

		if (!additive)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		

		// Vertex shader
		GLuint VP_Matrix_ID = glGetUniformLocation(programID, "VP");
		GLuint CamRight_ID = glGetUniformLocation(programID, "CamRight_W");
		GLuint CamUp_ID = glGetUniformLocation(programID, "CamUp_W");

		glUniformMatrix4fv(VP_Matrix_ID, 1, GL_FALSE, &vp_maxtrix[0][0]);
		glUniform3f(CamRight_ID, CameraRight_worldspace.x, CameraRight_worldspace.y, CameraRight_worldspace.z);
		glUniform3f(CamUp_ID, CameraUp_worldspace.x, CameraUp_worldspace.y, CameraUp_worldspace.z);

		//Particle Texture Binding
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRef);

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
			4, // size : x + y + z => 3
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

#pragma region Draw Particle Texture

		// 4th attribute buffer : particles' texture
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, particle_texture_buffer);
		glVertexAttribPointer(
			3, // attribute. No particular reason for 3, but must match the layout in the shader.
			2, // size : Two values per vertex are extracted. X and Y coordinates
			GL_FLOAT, // type
			GL_FALSE, // normalized
			0, // stride
			(void*)0 // array buffer offset
			);

#pragma endregion

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
		glVertexAttribDivisor(2, 1); // colour : one per quad -> 1
		glVertexAttribDivisor(3, 0); // 

		// Draw the actual buffers
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDepthMask(GL_TRUE);

		// Partial Cleanup, Disabling of VertexAttribArrays
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

	}
#pragma endregion

	// Unload particle shader program
	glUseProgram(0);
}

// Function to perform cleanup of buffers and texture allocations
void ParticleEmitter::Cleanup(){

	// CleanUp function
	// Cleanup VBO 
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteBuffers(1, &particle_texture_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &particles_colour_buffer);

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

	glUseProgram(0);
}


// Function to print out the number of Particles currently active
void ParticleEmitter::particleCount() {

	int count = 0;
	for (int i = 0; i < MAXPARTICLES; i++) {

		if (ParticleContainer[i].lifetime > 0) {
			count++;
		}
	}
	std::cout << "\nParticleCount: " << count << std::endl;
}

// Function to set all particles in the container dead.
void ParticleEmitter::disable() {

	for (int i = 0; i < MAXPARTICLES; i++) {

		ParticleContainer[i].lifetime = 0;
	}
	std::cout << "\nParticles Reset" << std::endl;

}

// Function to get the FPS 
float ParticleEmitter::whatIsFPS() {

	// FPS
	// Get the stored delta value [d_Store] - should be around 16 - 18ms
	// So 1/0.016 = ~60 frames per second
	return fps = 1 / ((d_Store/1000.0f));

	std::cout << "\nFPS: " << fps << std::endl;

}

// Function for Scaling the Particle size up
void ParticleEmitter::scaleBufferUp() {

	for (int i = 0; i < 12; i++)
	{
		g_vertex_buffer_data[i] = g_vertex_buffer_data[i] * 1.1f;
	}

}

// Function for Scaling the Particle size down
void ParticleEmitter::scaleBufferDown() {

	for (int i = 0; i < 12; i++)
	{
		g_vertex_buffer_data[i] = g_vertex_buffer_data[i] * 0.9f;
	}

}