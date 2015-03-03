#version 330 core

layout(location = 0) in vec3 vertexPosM;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 vertexNormal_modelspace;


out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec2 textureCoords;
out vec3 nColour;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

void main(){	
	// Position is the vertex position * matrix
	gl_Position =  MVP * vec4(vertexPosM ,1);
	
	// Get the vertex position in the worldspace, only use the model matrix as the camera position and perspective don't matter
	Position_worldspace = (M * vec4(vertexPosM,1)).xyz;
	
	// Get the vertex position relative to the camera
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosM,1)).xyz;

	// Get the direction that the camera is facing
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
	
	// Get the position of the light in camera space
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	
	// Get the direction of the light relative to the camera
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Get the value of the normal in camera space
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; 
	
	// Pass the texture coordinates straight through
	textureCoords = texCoords;
	
	nColour = vertexNormal_modelspace;
}
