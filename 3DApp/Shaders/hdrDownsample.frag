#version 400 core

in vec2 pass_TexCoord;

layout (location = 0) out vec4 out_Color;

uniform sampler2D tex;
uniform vec2 texSize;

void main(void)
{
	// Downsample.
	vec2 pixelSize = vec2(1.0 / texSize.x, 1.0 / texSize.y);
	vec2 textureCoordinates = pass_TexCoord;
	vec4 color = texture2D(tex, textureCoordinates);
	color += texture2D(tex, textureCoordinates + vec2( 1.0, 0.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2(-1.0, 0.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2( 0.0, 1.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2( 0.0, -1.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2( 1.0, 1.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2(-1.0, -1.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2( 1.0, -1.0) * pixelSize);
	color += texture2D(tex, textureCoordinates + vec2(-1.0, 1.0) * pixelSize);
	
	out_Color = color / 9.0;
}