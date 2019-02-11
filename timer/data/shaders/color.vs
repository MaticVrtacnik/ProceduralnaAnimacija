
//INPUTS=======================================================================
layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;
uniform vec3 u_color;


//CODE=========================================================================
void main()
{
	gl_Position = u_modelViewProjectionMatrix * vec4(i_vertex, 1.0);
}
