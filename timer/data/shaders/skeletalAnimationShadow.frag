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

//OUTPUTS======================================================================
layout (location = 0) out vec2 o_fragColor;


//CODE=========================================================================
void main()
{	
	float moment1 = gl_FragCoord.z;
	float dx = dFdx(moment1);
	float dy = dFdy(moment1);
	
	float moment2 = moment1 * moment1 + 0.25 * (dx * dx + dy * dy);

	o_fragColor = vec2(moment1, moment2);
	//o_fragColor = vec2(1.0);
	
	
	/*const float LIGHT_NEAR = 0.1f;
	const float LIGHT_FAR = 1000.0f;
	const float EXPONENT = 15.0f;

	float depthDivisor = (1.0 / gl_FragCoord.z);
	float mappedDivisor = map_01(depthDivisor, LIGHT_NEAR, LIGHT_FAR);
	 	
	o_fragColor.r = exp(EXPONENT * mappedDivisor);
	
	o_fragColor.r = linearizeDepth(gl_FragCoord.z);
	//o_fragColor.r = mappedDivisor;
	//o_fragColor.r = 0.5;*/
}
