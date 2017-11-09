#version 330 core

// Input
in vec2 UV;
in vec3 NORMAL;
in vec4 mShadowCoord;

// Ouput data
layout(location = 0) out vec4 COLOR;

uniform vec4 color;
uniform sampler2D diffuse;

uniform vec3 lightPos;
uniform vec4 lightAmbient;
uniform vec4 lightColor;

uniform sampler2DShadow shadowTex;

void main(){
	vec4 col = vec4(1.0);
	col = color * texture(diffuse, UV).rgba;
	
	float fDiffuseIntensity = max(0.0, dot(normalize(NORMAL), normalize(lightPos)));
	col = col * vec4((lightAmbient.rgb * lightAmbient.a) + (lightColor.rgb * lightColor.a * fDiffuseIntensity), 1.0);
	
	float mShadowBias = 0.005;
	float mShadowValue = texture(shadowTex, vec3(mShadowCoord.xy, (mShadowCoord.z - mShadowBias)/mShadowCoord.w));
	col.rgb *= lightAmbient.a+(mShadowValue*(1.0-lightAmbient.a));
	
	COLOR = col;
}