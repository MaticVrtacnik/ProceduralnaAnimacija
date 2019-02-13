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

#define NUM_HALF_DIR_SAMPLES 2
#define NUM_DIR_SAMPLES (NUM_HALF_DIR_SAMPLES * 2 + 1)
#define NUM_SAMPLES (NUM_DIR_SAMPLES * NUM_DIR_SAMPLES)

void main()
{
	vec2 pixelSize = 1.0 / textureSize(screenTexture, 0).xy;
	vec3 finalColor = vec3(0.0);
	
	#pragma unroll
	for(int i = -NUM_HALF_DIR_SAMPLES; i <= NUM_HALF_DIR_SAMPLES; ++i){
		for(int j = -NUM_HALF_DIR_SAMPLES; j <= NUM_HALF_DIR_SAMPLES; ++j){
			vec2 offset = vec2(pixelSize.x * float(j), pixelSize.y * float(i));
			finalColor += texture(screenTexture, outUV + offset).rgb;
		}
	}
	
	fragColor = vec4(finalColor.rgb / float(NUM_SAMPLES), 1.0);
}






