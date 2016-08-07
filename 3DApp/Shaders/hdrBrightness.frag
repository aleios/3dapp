#version 400 core

in vec2 pass_TexCoord;

layout (location = 0) out vec4 out_Color;

uniform sampler2D tex;

const float Threshold = 0.7;
const float Factor = 4.0;

void main(void)
{
	vec3 color = texture2D(tex, pass_TexCoord).rgb;
	
	/*
	color.r = clamp(color.r, 0.0, 1.0);
	color.g = clamp(color.g, 0.0, 1.0);
	color.b = clamp(color.b, 0.0, 1.0);*/
	
	
	vec4 sourceFragment = texture2D(tex, pass_TexCoord);
	float luminance = sourceFragment.r * 0.2126 + sourceFragment.g * 0.7152 + sourceFragment.b * 0.0722;
	sourceFragment *= clamp(luminance - Threshold, 0.0, 1.0) * Factor;
	out_Color = sourceFragment;
}