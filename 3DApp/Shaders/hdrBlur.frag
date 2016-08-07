#version 400 core

in vec2 pass_TexCoord;

layout (location = 0) out vec4 out_Color;

uniform sampler2D tex;
uniform vec2 offset;

uniform int lodlevel;

void main(void)
{
	vec2 textureCoordinates = pass_TexCoord;
	vec4 color = vec4(0.0);
	//float lodlevel = 2;
	color += textureLod(tex, textureCoordinates - 4.0 * offset, lodlevel) * 0.0162162162;
	color += textureLod(tex, textureCoordinates - 3.0 * offset, lodlevel) * 0.0540540541;
	color += textureLod(tex, textureCoordinates - 2.0 * offset, lodlevel) * 0.1216216216;
	color += textureLod(tex, textureCoordinates - offset, lodlevel) * 0.1945945946;
	color += textureLod(tex, textureCoordinates, lodlevel) * 0.2270270270;
	color += textureLod(tex, textureCoordinates + offset, lodlevel) * 0.1945945946;
	color += textureLod(tex, textureCoordinates + 2.0 * offset, lodlevel) * 0.1216216216;
	color += textureLod(tex, textureCoordinates + 3.0 * offset, lodlevel) * 0.0540540541;
	color += textureLod(tex, textureCoordinates + 4.0 * offset, lodlevel) * 0.0162162162;
	out_Color = color;
}