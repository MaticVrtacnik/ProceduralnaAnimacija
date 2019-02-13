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
layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv; //TODO maybe have to fix location ID

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_modelMatrix;
uniform mat3 u_normalMatrix;

//OUTPUTS======================================================================
out block{	
	vec3 vertex;
	vec3 normal;
	vec3 cubemapUv;
	vec2 uv;
}OUT;

//CODE=========================================================================
void main()
{	
	OUT.vertex = vec3(u_modelMatrix * vec4(i_vertex, 1.0));
	OUT.normal = u_normalMatrix * i_normal;
	OUT.cubemapUv = vec3(-i_vertex.x, -i_vertex.y, -i_vertex.z);
	OUT.uv = i_uv;
	
	gl_Position = u_modelViewProjectionMatrix * vec4(i_vertex, 1.0);
}
