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

#include "Node.h"

// Creation of Particle Structure
typedef struct Particle{

	// Particle Variables
#pragma region Variables
	// Variables for the structure
	float lifetime;				// Particle Lifetime
	float startLifetime;		// Initial value for lifetime
	glm::vec3 velocity;			// Particle Speed Vector
	glm::vec3 acc;				// Acceleration Vector
	glm::vec3 position;			// Particle Position
	float currentSize;			// Current Scale Value
	float endSize;				// Result of Scaling
	float startSize;			// Initial Scale Value
	glm::vec4 currentColour;	// Current Particle Colour
	glm::vec4 startColour;		// Starting Particle Colour
	glm::vec4 endColour;		// Ending Particle Colour
	float cameradistance;		// Distance from the camera. Set to -1 when dead. Used for sorting
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
		position = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
		acc = glm::vec3(0, 0, 0);
		lifetime = 0;
		currentSize = 0;
		startSize = 0;
		endSize = 0;
		cameradistance = -10;
	};

	// Functions for the Particles
#pragma region Particle Functions

	// Update particles
	void update(float delta){

		// Reduce lifetime in relation to time (60 ticks - 1.0 life)
		lifetime -= 0.016f*delta;

		// Update Velocity based off acceleration
		// Update Position based off velocity 
		velocity += acc;
		position += velocity * delta;

		// Get the particle's size and colour based on it's current lifetime
		interpolateScaleAndColour();
	};

	// Checks if a particle is dead
	bool isDead(){
		return (lifetime <= 0);
	};

	// Function to return the lifetime
	float getLifeTime(){
		return lifetime;
	};

	// Function to return the position of a particle
	glm::vec3 getPosition(){
		return position;
	};

	// Function calculates the particle's current size
	void interpolateScaleAndColour(){

		// Get the % of life used up
		float percent = 1-(lifetime/startLifetime);

		// Get the size to be added onto the starting size
		float addedSize = (endSize-startSize)*percent;

		// Get the colour to be added to the starting colour
		glm::vec4 addedColour = (endColour-startColour)*percent;

		// Set the colour 
		currentColour = startColour+addedColour;

		// Set the size
		currentSize = startSize+addedSize;
	}
#pragma endregion

}Particle; // end of Particle struct

//Particle Emitter Class
class ParticleEmitter
{
public:
	//Variables for the Emitter to modify
	float p_lifetime, 
		velocity, velocityRange,
		startScale, endScale,
		rate, rateCounter;
	glm::vec4 p_colourStart, p_colourEnd;
	glm::vec3 p_velocity, angleRange;
	glm::vec3 p_acc;
	glm::vec3 position;
	aiNode* sPos_Node;
	float emitterDir[3];
	
	// Default Constructor
	ParticleEmitter();

	// ParticleEmitter function, passes in the variables from above
	ParticleEmitter(GLuint shaderProgram, aiNode* node, std::string textureFile);

	//ParticleEmitter(GLuint shaderProgram, glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col);
	
	// ParticleEmitterUpdate function
	void Update(float delta, glm::mat4 view);

	// Draw Function
	void Draw(glm::mat4 viewMatrix, glm::mat4 vpmatrix);

	// CleanUp of Arrays
	void Cleanup();

	// Function for printing out the current particle count
	int particleCount();

	// Function to disable emitter (doesnt work)
	void disable();

	// Gets the FPS of the particle emitter
	float whatIsFPS();

	// Multiplies the buffer values by 1.1 to make the particle bigger
	void scaleBufferUp();

	// Multiplies the buffer values by 0.9 to make the particle smaller
	void scaleBufferDown();

	void StoreParticleTextureData(std::string fileRoot);

	GLuint textureRef;

	bool additive;

};