//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenGL Mathemathics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

// Standard Vector Library
#include <vector>

//#include "Particle.h"
#include "ParticleEmitter.h"

//Creation of Particle Structure
typedef struct Particle{
	Particle(){

	};

	//Variables for the structure
	float lifetime;
	glm::vec4 colour;
	glm::vec3 velocity;
	glm::vec3 acc;
	glm::vec3 position;

	//Initializing constructor
	Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life) {

		//Constructor:: sets variables for particles
		position = pos;
		velocity = vel;
		acc = accel;
		lifetime = life;

	};
	
	///Functions for the Particles

	//Update particles
	//only updates 'alive' particles
	void update(float delta){

		//Check Particle is alive
		if (lifetime > 0)
		{
			//gotta take away yo life fool
			//we dont know what this value is lol
			lifetime -= delta / 60;

			//Gotta go fast
			//Update Velocity based off acceleration
			//Update Position based off velocity 
			velocity += acc;
			position += velocity;
		}
	};

	//Checks if a particle is dead
	bool isDead(){
		return lifetime < 0;
	};

	//function to return the lifetime
	int getLifeTime(){
		return lifetime;
	};

	//function to return the position of a particle
	glm::vec3 getPosition(){
		return position;
	};

}Particle;

//Annoying
const static int MAXPARTICLES = 10;

//Gotta get that time yo
int oldTime = 0;

//Container For Particles
Particle ParticleContainer[MAXPARTICLES];

//Will hold positions for all current alive particles
glm::vec3 p_posArray[MAXPARTICLES];

//Constructor for Particle Emitter
//Passes through position, velocity, acceleration, lifeTime and colour
ParticleEmitter::ParticleEmitter(glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col){

	//Assignment of parameters
	position = pos;
	p_velocity = vel;
	p_acc = accel;
	p_lifetime = life;
	p_colour = col;

	//Loop to initialize all particles
	for (int i = 0; i < MAXPARTICLES; i++)
	{
		ParticleContainer[i] = Particle(position, p_velocity, p_acc, p_lifetime);
	}

}


void ParticleEmitter::PEmitterUpdate(){
	//updates
	//alive particles

	//wibbly wobbly timey wimey stuff
	int timeAtStart = glutGet(GLUT_ELAPSED_TIME);
	//remember that time rayquaza battled deoxys? DELTA EPISODE IN DE POKEMANS
	int delta = timeAtStart - oldTime;
	oldTime = timeAtStart;

	//Updates the ParticleContainer
	//Sets the values for p_posArray to the particle positions in ParticleContainer
	for (int i = 0; i < MAXPARTICLES; i++)
	{
		ParticleContainer[i].update(delta);
		p_posArray[i] = ParticleContainer[i].getPosition();
	}
}