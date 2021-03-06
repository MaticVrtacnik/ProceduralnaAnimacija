/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//CONSTS=======================================================================
#define NUM_SAMPLES 100

/*const float decay = 0.965;
const float density = 1.0;
const float weight = 20.0;*/

//INPUTS=======================================================================
in block{
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_occludedLightTexture;

//UNIFORMS=====================================================================
uniform vec2 u_screenLightLocation;
uniform float u_decay;
uniform float u_density;
uniform float u_weight;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{	
	vec2 currentUv = IN.uv;
	vec2 deltaUV = IN.uv - u_screenLightLocation;
	deltaUV *= 1.0 / float(NUM_SAMPLES) * u_density;
	
	float currentDensity = 1.0;
	vec3 finalColor = texture(u_occludedLightTexture, currentUv).rgb; 
	
	for(int i = 0; i < NUM_SAMPLES; ++i){
		currentUv -= deltaUV;
		
		vec3 color = texture(u_occludedLightTexture, currentUv).rgb;
		finalColor += color * currentDensity * u_weight;
		
		currentDensity *= u_decay;
	}
	
	finalColor /= float(NUM_SAMPLES);
	
	o_fragColor = vec4(finalColor, 1.0) * 1.5;
	//o_fragColor.rgb = dot(vec3(0.3333), finalColor.rgb).rrr;
	//o_fragColor *= 0.1;
	//o_fragColor = vec4(0.0);
	//o_fragColor = pow(o_fragColor, vec4(3.0)) * 25.0; 
}
