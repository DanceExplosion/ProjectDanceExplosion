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
	//Variables for the Emitter to modify
	int p_lifetime;
	glm::vec4 p_colour;
	glm::vec3 p_velocity;
	glm::vec3 p_acc;
	glm::vec3 position;

	//Value for max number of Particles in the system
	//10 is hardcore
	const static int MaxParticles = 10;
	
	//ParticleEmiotter function, passes in the variables from above
	ParticleEmitter(glm::vec3 pos, glm::vec3 vel, glm::vec3 accel, float life, glm::vec4 col);

	//Destructor for ParticleEmitter
	~ParticleEmitter();
	
	//ParticleEmitterUpdate function
	void PEmitterUpdate();

};