#version 400

in vec2 pass_TexCoord;
in vec3 pass_Normal;
in vec3 pass_Position;

in vec2 pass_Depth;

uniform sampler2D watertex;
uniform sampler2D groundtex;

in float height;

layout (location = 0) out vec4 out_Diffuse;
layout (location = 1) out vec4 out_Normal;
layout (location = 2) out vec4 out_Position;

uniform float specularIntensity;
uniform float specularPower;

#define kPI 3.1415926536f
vec2 EncodeNormal(in vec3 n)
{
	return vec2(n.xy / sqrt(n.z * 8 + 8) + 0.5);
}

void main(void)
{
	// Diffuse
	out_Diffuse.rgb = texture2D(watertex, pass_TexCoord.st).rgb;

	
	// Specular intensity.
	out_Diffuse.a = specularIntensity;

	// Normals
	out_Normal.xy = EncodeNormal(pass_Normal);
	
	// Specular power
	out_Normal.b = specularPower;
	
	// Position
	//out_Position = vec4(pass_Position, 1.0);
	out_Position.r = pass_Depth.x / pass_Depth.y;
}