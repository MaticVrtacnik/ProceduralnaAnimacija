
//INPUTS=======================================================================
layout (location = 0) in vec3 i_vertex;

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;


//CODE=========================================================================
void main()
{
	gl_Position = u_modelViewProjectionMatrix * vec4(i_vertex, 1.0);
}
