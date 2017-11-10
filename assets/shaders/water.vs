#version 330 core

layout(location = 0) in vec3 vertexPosition;

// Output
out vec2 mTextureCoords;
out vec4 mWaterCoord;
out vec4 mShadowCoord;

// Uniforms
uniform mat4 mModelViewProjection;
uniform vec2 mUVSize;
uniform mat4 shadowMatrix;

void main(){
	// Vertex position
	gl_Position = mModelViewProjection * vec4(vertexPosition, 1.0);
	
	// Texcoords
	mTextureCoords = ((vertexPosition.xz + vec2(1, 1)) / 2.0) * mUVSize / 1.5;
	mWaterCoord = mModelViewProjection * vec4(vertexPosition.x, 0.0, vertexPosition.z, 1.0);
	
	// ShadowCoord
	mShadowCoord = shadowMatrix * vec4(vertexPosition, 1.0);
}
