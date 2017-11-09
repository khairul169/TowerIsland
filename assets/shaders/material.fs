#version 330 core

// Input
in vec2 UV;
in vec3 NORMAL;

// Ouput data
layout(location = 0) out vec4 COLOR;

uniform vec4 color;
uniform sampler2D diffuse;

uniform vec3 lightPos;
uniform vec4 lightAmbient;
uniform vec3 lightColor;

void main(){
	vec4 col = vec4(1.0);
	col = color * texture(diffuse, UV).rgba;
	
	/*if (col.a < 0.2) {
		discard;
	}*/
	
	float fDiffuseIntensity = max(0.0, dot(normalize(NORMAL), normalize(lightPos)));
	col = col * vec4((lightAmbient.rgb * lightAmbient.a) + (lightColor * fDiffuseIntensity), 1.0);
	//col.a = 1.0;
	
	COLOR = col;
}