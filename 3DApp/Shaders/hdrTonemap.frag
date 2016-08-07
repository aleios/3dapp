#version 400 core

in vec2 pass_TexCoord;

out vec4 out_Color;

uniform sampler2D tex;
uniform sampler2D tex2;

uniform float exposure;

vec4 Tonemap(vec3 col)
{
	float f0 = 0.15; // A
	float f1 = 0.50; // B
	float f2 = 0.10; // C
	float f3 = 0.20; // D
	float f4 = 0.02; // E
	float f5 = 0.30; // F
	
	return vec4(((col * (f0*col + f2*f1) + f3*f4) / (col * (f0*col + f1) + f3*f5)) - f4/f5, 1.0); 
}

// Combine blur and final scene then tone map them.
void main(void)
{
	vec3 sceneCol = texture2D(tex, pass_TexCoord.st).rgb + texture2D(tex2, pass_TexCoord.st).rgb;
	vec3 whitePoint = vec3(10.0f, 10.0f, 10.0f);
	out_Color = Tonemap(sceneCol * exposure) / Tonemap(whitePoint);
}