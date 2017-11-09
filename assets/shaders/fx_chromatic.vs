#version 330 core

// Vertex data
layout(location = 0) in vec3 vertexPosition;

// Output
out vec2 UV;

uniform mat4 mProjection;

void main(){
	gl_Position = mProjection * vec4(vertexPosition, 1.0);
	UV = vertexPosition.xy;
}
