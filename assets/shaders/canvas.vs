#version 330 core

// Vertex data
layout(location = 0) in vec3 vertexPosition;

// Output
out vec2 UV;

uniform mat4 modelProjection;

void main(){
	gl_Position =  modelProjection * vec4(vertexPosition, 1.0);
	UV = vertexPosition.xy;
}
