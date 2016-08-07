#version 400

uniform sampler2D tex;

in vec3 pass_Position;
in vec4 pass_Color;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
	out_Color = texture2D(tex, pass_TexCoord);
}