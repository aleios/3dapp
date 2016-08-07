///
///DIFFER IMPL
/// ref[1]: http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
/// ref[2]: https://github.com/bartwronski/CSharpRenderer/blob/master/shaders/FXAA.hlsl
/// Adapted both for GLSL and OpenGL 4.0 standards

#version 400 core

in vec2 pass_TexCoord;
out vec4 out_Color;

uniform sampler2D tex;
uniform vec2 screenSize;

// FXAA Preset
#define FXAA_EDGE_THRESHOLD (1.0 / 8.0)
#define FXAA_EDGE_THRESHOLD_MIN (1.0 / 16.0)
#define FXAA_SUBPIX 1
#define FXAA_SUBPIX_TRIM (1.0 / 4.0)
#define FXAA_SUBPIX_TRIM_SCALE (1.0 / 4.0)
#define FXAA_SUBPIX_CAP (3.0 / 4.0)
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_STEPS (16 * FXAA_SEARCH_ACCELERATION)
#define FXAA_SEARCH_THRESHOLD (1.0 / 4.0)

float LumaOutput(vec3 rgbVal)
{
	return rgbVal.y * (0.587 / 0.299) + rgbVal.x;
}

vec3 FXAAOutput()
{
	// FORMAT: (tex, pos, lod, offset)
	vec3 rgb = texture2D(tex, pass_TexCoord).xyz;
	vec3 rgbN = texture2D(tex, pass_TexCoord + vec2(0.0, -1.0)).xyz;
	vec3 rgbS = texture2D(tex, pass_TexCoord + vec2(0.0, 1.0)).xyz;
	vec3 rgbE = texture2D(tex, pass_TexCoord + vec2(1.0, 0.0)).xyz;
	vec3 rgbW = texture2D(tex, pass_TexCoord + vec2(-1.0, 0.0)).xyz;
	
	// LUMA
	float luma = LumaOutput(rgb);
	float lumaN = LumaOutput(rgbN);
	float lumaS = LumaOutput(rgbS);
	float lumaE = LumaOutput(rgbE);
	float lumaW = LumaOutput(rgbW);
	
	float rangeMin = min(luma, min(min(lumaN, lumaW), min(lumaS, lumaE)));
	float rangeMax = max(luma, max(max(lumaN, lumaW), max(lumaS, lumaE)));
	float range = rangeMax - rangeMin;
	
	// Early exit
	if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
		return rgb;
		
	// Subpixel Aliasing
	#if FXAA_SUBPIX > 0
		vec3 rgbL = rgb + rgbN + rgbS + rgbE + rgbW;
	#endif
	
	#if FXAA_SUBPIX != 0
		float lumaL = (lumaN + lumaS + lumaE + lumaW) * 0.25;
		float rangeL = abs(lumaL - luma);
	#endif
	#if FXAA_SUBPIX == 1
		float blendL = max(0.0, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
		blendL = min(FXAA_SUBPIX_CAP, blendL);
	#endif
	
	// Horiz/Vert search
	vec3 rgbNW = texture2D(tex, pass_TexCoord + vec2(-1.0, -1.0)).xyz;
	vec3 rgbNE = texture2D(tex, pass_TexCoord + vec2(1.0, -1.0)).xyz;
	vec3 rgbSW = texture2D(tex, pass_TexCoord + vec2(-1.0, 1.0)).xyz;
	vec3 rgbSE = texture2D(tex, pass_TexCoord + vec2(1.0, 1.0)).xyz;
	
	#if FXAA_SUBPIX > 0
		rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
		rgbL *= (1.0 / 9.0);
	#endif
	
	float lumaNW = LumaOutput(rgbNW);
	float lumaNE = LumaOutput(rgbNE);
	float lumaSW = LumaOutput(rgbSW);
	float lumaSE = LumaOutput(rgbSE);
	
	// Vertical Edge
	float edgeVert = abs((0.25 * lumaNW) + (- 0.5 * lumaN) + (0.25 * lumaNE)) + abs((0.50 * lumaW ) + (- 1.0 * luma) + (0.50 * lumaE )) + abs((0.25 * lumaSW) + (- 0.5 * lumaS) + (0.25 * lumaSE));
	
	// Horizontal Edge
	float edgeHorz = abs((0.25 * lumaNW) + (- 0.5 * lumaW) + (0.25 * lumaSW)) + abs((0.50 * lumaN ) + (- 1.0 * luma) + (0.50 * lumaS )) + abs((0.25 * lumaNE) + (- 0.5 * lumaE) + (0.25 * lumaSE));
	
	// Calculte span.
	bool horzSpan = edgeHorz >= edgeVert;
	
	vec2 posP = posN;
	
	// Algorithm taken directly from ref[1]
	for(uint i = 0; i < FXAA_SEARCH_STEPS; i++) {
		#if FXAA_SEARCH_ACCELERATION == 1
			if(!doneN) lumaEndN = LumaOutput(texture2D(tex, posN.xy).xyz);
			if(!doneP) lumaEndP = LumaOutput(texture2D(tex, posP.xy).xyz);
		#else
			if(!doneN) lumaEndN = LumaOutput(texture2DGrad(tex, posN.xy, offNP).xyz);
			if(!doneP) lumaEndP = LumaOutput(texture2DGrad(tex, posP.xy, offNP).xyz);
		#endif
		doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
		doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
		if(doneN && doneP) break;
		if(!doneN) posN -= offNP;
		if(!doneP) posP += offNP; 
	}
	
	float spanLength = (dstP + dstN);
	dstN = directionN ? dstN : dstP;
	float subPixelOffset = (0.5 + (dstN * (-1.0 / spanLength))) * lengthSign;
	
	vec3 rgbF = texture2D(tex, vec2(
		pos.x + (horzSpan ? 0.0 : subPixelOffset),
		pos.y + (horzSpan ? subPixelOffset : 0.0))).xyz;
	
	#if FXAA_SUBPIX == 0
		return rgbF;
	#else
		return (vec3(-blendL) * rgbF) + ((rgbL * vec3(blendL)) + rgbF);
	#endif
}

void main(void)
{
	out_Color = vec4(FXAAOutput(), 1.0);
}
