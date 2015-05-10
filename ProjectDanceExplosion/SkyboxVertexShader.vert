#version 330 core

layout(location = 0) in vec3 vertexPosM;
layout(location = 1) in vec2 texCoords;

out vec2 textureCoords;

uniform mat4 VP;
uniform vec3 vPos;

void main(){	

	// Position is the vertex position * matrix
	gl_Position =  VP * vec4((vertexPosM+(vPos/1.05)) ,1);
	
	

	// Pass the texture coordinates straight through
	textureCoords = texCoords;
	
}
