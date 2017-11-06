#version 330 core

// Input
in vec2 UV;
in vec3 NORMAL;

// Output
out vec3 COLOR;

uniform vec3 color;
uniform sampler2D diffuse;

uniform vec3 lightPos;
uniform vec4 lightAmbient;
uniform vec3 lightColor;

void main(){
	vec3 col = vec3(1.0);
	col = color * texture(diffuse, UV).rgb;
	
	float fDiffuseIntensity = max(0.0, dot(normalize(NORMAL), normalize(lightPos)));
	col = col * ((lightAmbient.rgb * lightAmbient.a) + (lightColor * fDiffuseIntensity));
	
	COLOR = col;
}