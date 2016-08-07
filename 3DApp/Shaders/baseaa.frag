#version 400 core

in vec2 pass_TexCoord;
out vec4 out_Color;

uniform sampler2D tex;
uniform vec2 screenSize;

float Threshold(in float a, in float b, in float value)
{
	if(a > value) return 0.0;
	if(b < value) return 1.0;
	return value;
}

vec3 SharpenOutput(in vec3 input)
{
	float sFactor = 20.0;
	vec3 col = input;
	
	col -= texture2D(tex, pass_TexCoord + 0.0001).rgb * sFactor;
	col += texture2D(tex, pass_TexCoord - 0.0001).rgb * sFactor;
	
	return col;
}

float DetectEdge(in vec2 coord)
{
	return 0.0;
}

float get_average(in vec3 color)
{
	return (color.r + color.b + color.g) / 3.0;
}

vec3 DoAA()
{
	// Get middle fragment
	vec3 rgbMid = texture2D(tex, pass_TexCoord).rgb;
	
	// Get neighbours
	float tx = 1.0 / screenSize.x;
	float ty = 1.0 / screenSize.y;
	vec3 rgbN  = texture2D(tex, pass_TexCoord + vec2(0, -1 * ty)).rgb;
	vec3 rgbS  = texture2D(tex, pass_TexCoord + vec2(0, 1 * ty)).rgb;
	vec3 rgbE  = texture2D(tex, pass_TexCoord + vec2(1 * tx, 0)).rgb;
	vec3 rgbW  = texture2D(tex, pass_TexCoord + vec2(-1 * tx, 0)).rgb;
	vec3 rgbNW = texture2D(tex, pass_TexCoord + vec2(-1 * tx, -1 * ty)).rgb;
	vec3 rgbNE = texture2D(tex, pass_TexCoord + vec2(1 * tx, -1 * ty)).rgb;
	vec3 rgbSW = texture2D(tex, pass_TexCoord + vec2(-1 * tx, 1 * ty)).rgb;
	vec3 rgbSE = texture2D(tex, pass_TexCoord + vec2(1 * tx, 1 * ty)).rgb;
	
	// Detect edges

	
	float avgN = get_average(rgbN);
	float avgS = get_average(rgbS);
	float avgE = get_average(rgbE);
	float avgW = get_average(rgbW);
	float avgNE = get_average(rgbNE);
	float avgNW = get_average(rgbNW);
	float avgSE = get_average(rgbSE);
	float avgSW = get_average(rgbSW);
	
	float diff = (abs(avgN - avgS) + abs(avgE - avgW) +
				  abs(avgNW - avgSE) + abs(avgNE - avgSW)) / 4.0;
	
	
	float edge = Threshold(0.25, 0.4, clamp(1.8 * diff, 0.0, 1.0));
	
	// If rgbMid is an edge then we perform the AA on it. Otherwise just early exit.
	if(edge <= 0.0)
		return rgbMid;
	
	// Take the average for each component
	vec3 rgbA = (rgbMid + rgbNW + rgbNE + rgbSW + rgbSE + rgbN + rgbS + rgbE + rgbW) / 9.0;
	
	// Lowpass filter
	// This removes the high frequency noise causing aliasing.
	vec3 lrgbMid = rgbMid * 0.5;
	
	vec3 lrgbN = rgbN * 0.125;
	vec3 lrgbS = rgbS * 0.125;
	vec3 lrgbE = rgbE * 0.125;
	vec3 lrgbW = rgbW * 0.125;
	
	vec3 lrgbNW = rgbNW * 0.0625;
	vec3 lrgbNE = rgbNE * 0.0625;
	vec3 lrgbSW = rgbSW * 0.0625;
	vec3 lrgbSE = rgbSE * 0.0625;
	
	vec3 lrgb = (lrgbMid + lrgbNW + lrgbNE + lrgbSW + lrgbSE + lrgbN + lrgbS + lrgbE + lrgbW) / 9.0;
	
	// Blur
	// Bleed pixels onto neighbours.
	vec3 brgb = mix(lrgb, rgbA, vec3(0.7, 0.7, 0.7));
	
	// Sharpen
	// We are done with noise removal and should have a decent set of edge blur. Sharpen the image.
	brgb = SharpenOutput(brgb);
	
	// Compare against original
	brgb = mix(brgb, rgbMid, vec3(0.2, 0.2, 0.2));
	
	// Return the result
	return brgb;
}

void main(void)
{
	out_Color = vec4(DoAA(), 1.0);
}