#version 400

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Samplers
uniform sampler2D difftex;
uniform sampler2D normtex;
uniform sampler2D postex;

// Fragment inputs
uniform vec2 screenSize;

// Light properties
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientIntensity;
uniform float diffuseIntensity;

uniform float attenuationConstant;
uniform float attenuationLinear;
uniform float attenuationExponential;

// Camera
uniform vec3 eyePosition;

// Output
in vec4 pass_ScreenPos;
out vec4 out_Color;

// Calculate the tex coords to use for our information textures.
vec2 CalculateTexCoords()
{
	return gl_FragCoord.xy / screenSize;
}

#define kPI 3.1415926536f
vec3 DecodeNormal(vec2 enc)
{
	vec2 fenc = enc * 4 - 2;
	float f = dot(fenc, fenc);
	float g = sqrt(1 - f / 4);
	vec3 n;
	n.xy = fenc * g;
	n.z = 1 - f / 2;
	return n;
}

// Calculate our point light.
vec4 CalculateLight(in vec3 pos, in vec4 normal, in vec4 diffuse)
{
	// Get the normal and normalize it.
	vec3 Normal = DecodeNormal(normal.rg);
	Normal = normalize(Normal);
	
	// Get the specular material data.
	float materialSpecularIntensity = diffuse.a;
	float materialSpecularPower = normal.b;

	// Get the direction and distance of the light.
	vec3 lightDir = pos - lightPos;
	float dist = length(lightDir);
	lightDir = normalize(lightDir);
	
	// Determine ambient influence.
	vec4 ambientCol = vec4(lightColor.xyz, 1.0) * ambientIntensity;
	
	// Get the diffuse amount.
	float diff = dot(Normal, -lightDir);
	
	vec4 diffuseCol = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specCol = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(diff > 0.0)
	{
		diffuseCol = vec4(lightColor.xyz, 1.0) * diffuseIntensity * diff;
		
        vec3 worldEye = normalize(eyePosition - pos);
        vec3 R = normalize(reflect(-lightDir, Normal));
        float specFactor = pow(max(0.0, dot(worldEye, R)), materialSpecularPower);
        if(specFactor > 0.0)
		{
            specCol = vec4(lightColor.xyz, 1.0) * materialSpecularIntensity * specFactor;
        }
	}
	
	// Attenuation Calculation.
    float atten =  attenuationConstant +
                   attenuationLinear * dist +
                   attenuationExponential * dist * dist;

	// Limit attenuation.
    atten = max(1.0, atten);
	
	// Add resulting colors and scale by attenuation.
	return (ambientCol + diffuseCol + specCol) / atten;
}

void main(void)
{
	// Get the texture coordinates for our data.
	vec2 texCoord = CalculateTexCoords();

	// Sample the encoded data from the GBuffer.
	vec4 diffuse = texture2D(difftex, texCoord); // Provides color information.
	vec4 normal = texture2D(normtex, texCoord); // Provides eye space normal information.
	
	//vec3 pos = texture2D(postex, texCoord).xyz; // Provides eye space position information.
	vec4 posn;
	vec4 screenPos = pass_ScreenPos;
	screenPos.xy /= screenPos.w;
	
	posn.xy = screenPos.xy;
	posn.z = texture2D(postex, texCoord).r;
	posn.w = 1.0f;
	
	posn = inverse(projectionMatrix * viewMatrix) * posn;
	posn /= posn.w;
	
	// Final color output.
	out_Color = vec4(diffuse.xyz, 1.0) * CalculateLight(posn.xyz, normal, diffuse);
}