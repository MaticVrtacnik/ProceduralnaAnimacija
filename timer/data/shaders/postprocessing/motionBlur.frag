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


layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D velocityTexture;

//uniform float velocityScale; //TODO adjust for FPS
const int MAX_SAMPLES = 8;

uniform float fpsFactor;

void main()
{	
	vec2 pixelSize = 1.0 / textureSize(screenTexture, 0);
	
	vec2 velocity = texture(velocityTexture, outUV).rg;
	velocity *= 0.3/*fpsFactor*/; //0.5
	
	float speed = length(velocity / pixelSize);
	int numSamples = clamp(int(speed), 1, MAX_SAMPLES);
	
	vec3 finalColor = texture(screenTexture, outUV).rgb;
	for(int i = 1; i < numSamples; ++i){
		vec2 offset = velocity * (float(i) / float(numSamples - 1) - 0.5);
		finalColor += texture(screenTexture, outUV + offset).rgb;
	}
	
	finalColor /= float(numSamples);
	fragColor = vec4(finalColor, 1.0);
}






