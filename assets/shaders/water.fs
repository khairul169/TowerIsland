#version 330 core

// Input
in vec2 mTextureCoords;
in vec4 mWaterCoord;
in vec4 mShadowCoord;

// Ouput data
layout(location = 0) out vec4 COLOR;

uniform float mTime;
uniform vec4 mColor;
uniform sampler2D mReflectionTex;
uniform sampler2D mRefractionTex;
uniform sampler2D mDuDvMap;

// Shadow mapping
uniform sampler2DShadow shadowTex;

void main(){
	vec2 mWaterUV = (mWaterCoord.xy/mWaterCoord.w) / 2.0 + 0.5;
	vec4 col = mColor;
	
	vec2 mReflectionUV = mWaterUV;
	vec2 mRefractionUV = mWaterUV;
	
	// Flip reflection uv
	mReflectionUV.y = 1.0-mReflectionUV.y;
	
	vec2 mDistortion = texture(mDuDvMap, vec2(mTextureCoords.x, mTextureCoords.y)).rg * 2.0 - 1.0;
	mDistortion *= 0.01;
	
	mReflectionUV += mDistortion;
	mRefractionUV += mDistortion;
	
	vec3 reflection = texture(mReflectionTex, mReflectionUV).rgb;
	vec3 refraction = texture(mRefractionTex, mRefractionUV).rgb;
	col.rgb = col.rgb * mix(reflection, refraction, 0.6);
	
	float mShadowBias = 0.005;
	float mShadowValue = texture(shadowTex, vec3(mShadowCoord.xy, (mShadowCoord.z - mShadowBias)/mShadowCoord.w));
	col.rgb *= 0.75 + (mShadowValue * 0.25);
	
	COLOR = col;
}