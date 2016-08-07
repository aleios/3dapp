#version 400

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec3 in_Tangent;
layout(location=3) in vec3 in_Bitangent;
layout(location=4) in vec2 in_TexCoord;
layout(location=5) in vec4 in_Color;

out vec2 pass_TexCoord;
out vec3 pass_Normal;
out vec3 pass_Position;
out vec3 pass_ColPos;

out vec2 pass_Depth;

out float height;

uniform float time;

// hash based 3d value noise
// function taken from https://www.shadertoy.com/view/XslGRr
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}
/**********************************************/

float terrainGeneration(float x, float y)
{
	int octaves = 8;
	float freq = 1.0;
	float amp = 1.0;
	float v = 0;
	for(int i = 0; i < octaves; i++)
	{
		v += amp * noise(vec3((freq * (i + 1)) * x, (freq * (i + 1)) * y, 0.0));
		freq = freq * 1.2;
		amp = amp * 1.6;
	}
	
	return v;
}

vec3 terrainNormal (float x, float y)
{
  float delta = 0.1;
  vec3 A = vec3 (delta, terrainGeneration (x + delta, y) - terrainGeneration (x, y), 0);
  vec3 B = vec3 (0, terrainGeneration (x, y + delta) - terrainGeneration (x, y), delta);
  return normalize(cross(B, A));
}

void main(void)
{
	pass_TexCoord = in_TexCoord;
	pass_ColPos = in_Position;
	
	// Terrain generation
	vec3 Position = in_Position;
	
	height = terrainGeneration(time + Position.x, time + Position.z);
	Position.y = (0.3 * height) - 10.0;
	
	vec3 Normal = terrainNormal(Position.x, Position.z);
	//vec3 Normal = in_Normal;
	
	// Pass in model space.
	pass_Position = (modelMatrix * vec4(Position, 1.0)).xyz;
	pass_Normal = (modelMatrix * vec4(Normal, 0.0)).xyz;

	// Vertex Position Transform.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(Position, 1.0);
	
	pass_Depth.x = gl_Position.z;
	pass_Depth.y = gl_Position.w;
}