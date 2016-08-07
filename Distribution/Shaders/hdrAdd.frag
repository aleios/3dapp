#version 400 core

in vec2 pass_TexCoord;

layout (location = 0) out vec4 out_Color;

uniform sampler2D tex;
uniform sampler2D tex2;

void main(void)
{
	out_Color = texture2D(tex, pass_TexCoord) + texture2D(tex2, pass_TexCoord);
}