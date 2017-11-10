#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// Output
out vec2 UV;
out vec3 NORMAL;
out vec4 mShadowCoord;

// Uniforms
uniform mat4 uModelMatrix;
uniform mat4 modelViewProjection;
uniform mat4 normal;
uniform mat4 shadowMatrix;

uniform vec4 mClippingPlane;

void main(){
	vec4 worldPosition = uModelMatrix * vec4(vertexPosition, 1.0);
	
	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
	gl_ClipDistance[0] = dot(mClippingPlane, worldPosition);
	
	UV = vertexUV;
	NORMAL = (normal*vec4(vertexNormal, 0.0)).xyz;
	
	// ShadowCoord
	mShadowCoord = shadowMatrix * vec4(vertexPosition, 1.0);
}
