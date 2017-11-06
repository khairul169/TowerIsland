#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

// Output
out vec2 UV;

// Uniforms
uniform mat4 modelViewProjection;

void main(){
	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
	
	UV = vertexUV;
}
