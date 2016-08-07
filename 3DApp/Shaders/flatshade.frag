#version 400

in vec3 pass_Position;
in vec4 pass_Color;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(pass_Position, 1.0);//pass_Color;
}