#version 330 core

in vec2 textureCoords;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 nColour;

out vec4 color;

uniform sampler2D textureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main(){

	vec4 LightColor = vec4(0.5,0.5,0.5,1);
	float LightPower = 50;
	
	// Basic colour of the fragment
	vec4 MaterialDiffuseColor = texture2D( textureSampler, textureCoords );
	
	// The lowest possible value allowed for the texture, a darkened version of the diffuse
	vec4 MaterialAmbientColor = vec4(0.3,0.3,0.3,0.3) * MaterialDiffuseColor;
	
	// The colour of the specular highlights on the model (white)
	vec4 MaterialSpecularColor = vec4(0.3,0.3,0.3, 0.3);
	
	// Distance from the light to the position of the fragment
	float distance = length( LightPosition_worldspace - Position_worldspace );
	
	// Normalized cameraspace normal
	vec3 n = normalize( Normal_cameraspace );
	
	// Normalized light direction in cameraspace
	vec3 l = normalize( LightDirection_cameraspace );
	
	// Cosine of the angle of the dot product of the normal and light
	// Returns a value between 1 and 0, as clamp prevents it from being less than 0
	float cosTheta = clamp(dot( n,l ), 0,1 );
 	
	// Normalized eye direction in camera space
	vec3 E = normalize(EyeDirection_cameraspace);
	
	// Reflected normal in camera space
	vec3 R = reflect(-l,n);

	// Cosine of the dp of the eye and reflection vectors
	// Clamped to 0 again, used for specular higlights
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	// Pass the colour through after the final calculation
	// Diffuse and Specular are lit depending on their distance and the power of the camera, as well as
	// the angle of the fragment
	color = MaterialAmbientColor +

		(MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance)) +

		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}