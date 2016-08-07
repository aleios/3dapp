#version 400 core

in vec2 pass_TexCoord;

out vec4 out_Color;

uniform sampler2D tex;

void main(void)
{
	out_Color = texture2D(tex, pass_TexCoord.st);
}