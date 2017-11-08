#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;
uniform float time;

float linearizeDepth(vec2 uv)
{
  float n = 0.1;
  float f = 100.0;
  float z = texture(depthTexture, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));	
}

vec2 barrelDistortion(vec2 coord, float amt) {
	vec2 cc = coord - 0.5;
	float dist = dot(cc, cc);
	return coord + cc * dist * amt;
}

float sat( float t )
{
	return clamp( t, 0.0, 1.0 );
}

float linterp( float t ) {
	return sat( 1.0 - abs( 2.0*t - 1.0 ) );
}

float remap( float t, float a, float b ) {
	return sat( (t - a) / (b - a) );
}

vec4 spectrum_offset( float t ) {
	vec4 ret;
	float lo = step(t,0.5);
	float hi = 1.0-lo;
	float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
	ret = vec4(lo,1.0,hi, 1.) * vec4(1.0-w, w, 1.0-w, 1.);

	return pow( ret, vec4(1.0/2.2) );
}

const float max_distort = 0.2;
const int num_iter = 12;
const float reci_num_iter_f = 1.0 / float(num_iter);

void main(){
	// begin bloom shader
	/*
	vec3 sum = vec3(0.0);
	vec2 texcoord = UV;
	int j;
	int i;

	for( i= -4 ;i < 4; i++)
	{
		for (j = -3; j < 3; j++)
		{
			sum += texture(renderedTexture, texcoord + vec2(j, i)*0.004).rgb * 0.2;
		}
	}
	
	if (texture(renderedTexture, texcoord).r < 0.3)
	{
		color = sum * sum * 0.012 + texture(renderedTexture, texcoord).rgb;
	}
	else
	{
		if (texture(renderedTexture, texcoord).r < 0.5)
		{
			color = sum * sum * 0.009 + texture(renderedTexture, texcoord).rgb;
		}
		else
		{
			color = sum * sum * 0.0075 + texture(renderedTexture, texcoord).rgb;
		}
	}
	*/
	
	//color = texture( renderedTexture, UV).rgb;
	//color = vec3(linearizeDepth(UV));
	
	vec4 sumcol = vec4(0.0);
	vec4 sumw = vec4(0.0);	
	for ( int i=0; i<num_iter;++i )
	{
		float t = float(i) * reci_num_iter_f;
		vec4 w = spectrum_offset( t );
		sumw += w;
		sumcol += w * texture( renderedTexture, barrelDistortion(UV, .6 * max_distort*t ) );
	}
		
	color = (sumcol / sumw).rgb;
}