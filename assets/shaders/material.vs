#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// Output
out vec2 UV;
out vec3 NORMAL;
out vec4 mShadowCoord;

// Uniforms
uniform mat4 modelViewProjection;
uniform mat4 normal;
uniform mat4 shadowMatrix;

void main(){
	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
	
	UV = vertexUV;
	NORMAL = (normal*vec4(vertexNormal, 0.0)).xyz;
	
	// Shadow's coord
	mShadowCoord = shadowMatrix * vec4(vertexPosition, 1.0);
}
