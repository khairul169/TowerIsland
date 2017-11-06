#version 330 core

in vec2 UV;

out vec3 COLOR;

uniform vec3 color;
uniform sampler2D diffuse;

void main(){
	vec3 col = vec3(1.0);
	col = color * texture(diffuse, UV).rgb;
	COLOR = col;
}