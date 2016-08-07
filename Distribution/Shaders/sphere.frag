#version 400

in vec2 pass_TexCoord;
in vec3 pass_Normal;
in vec3 pass_Position;
in vec3 pass_ViewNormal;

in vec2 pass_Depth;

layout (location = 0) out vec4 out_Diffuse;
layout (location = 1) out vec4 out_Normal;
layout (location = 2) out vec4 out_Position;

uniform float specularIntensity;
uniform float specularPower;

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

vec3 GenerateColor(vec3 coords)
{
	float rchannel = noise(coords.xyz) * noise(coords.yxx);
	float gchannel = noise(coords.yxz);
	float bchannel = noise(coords.zyx);
	
	return vec3(rchannel, gchannel, bchannel);
}

#define kPI 3.1415926536f
vec2 EncodeNormal(in vec3 n)
{
	return vec2(n.xy / sqrt(n.z * 8 + 8) + 0.5);
}

void main(void)
{
	// Diffuse
	out_Diffuse.rgb = GenerateColor(pass_ViewNormal.xyz);
	
	// Specular intensity.
	out_Diffuse.a = specularIntensity;

	// Normals
	out_Normal.rg = EncodeNormal(pass_Normal);
	
	// Specular power
	out_Normal.b = specularPower;
	
	// Position
	//out_Position = vec4(pass_Position, 1.0);
	out_Position.r = pass_Depth.x / pass_Depth.y;
}