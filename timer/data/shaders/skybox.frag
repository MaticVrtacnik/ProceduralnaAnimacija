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
#include "include/commons.glsl"

//INPUTS=======================================================================
in block{
	vec3 vertex;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform samplerCube u_texture;

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out vec4 o_bloomColor;


//CODE=========================================================================
void main()
{
	vec3 textureColor = textureLod(u_texture, IN.vertex, 0).rgb;
	o_fragColor = vec4(pow(textureColor, vec3(2.2)), 1.0);
	
	const float brigthnessFalloff = 1.0;
	float brightness = pow(getBrightness(o_fragColor.rgb), brigthnessFalloff);
	//float brightness = min(1.0, pow(getBrightness(o_fragColor), brigthnessFalloff));
	
	o_bloomColor = vec4(o_fragColor.rgb * (0.2 + brightness), 1.0);
	//o_bloomColor = vec4(o_fragColor.rgb, 1.0);
	//o_bloomColor = vec4(0, 0, 0, 1);
}
