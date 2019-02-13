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

//INPUTS=======================================================================
layout (location = 0) in vec2 i_vertex;

//UNIFORMS=====================================================================
uniform mat4 u_projectionMatrix;
uniform ivec2 u_location;
uniform ivec2 u_scale;

//OUTPUTS======================================================================
out block{
	vec2 uv;
}OUT;


//CODE=========================================================================
void main()
{	
	OUT.uv = i_vertex + vec2(0.5, 0.5);
	OUT.uv.y = 1.0 - OUT.uv.y;
	
	vec2 vertex = i_vertex * u_scale + u_scale * 0.5 + u_location;	
	gl_Position = u_projectionMatrix * vec4(vertex, 0.0, 1.0);
}
