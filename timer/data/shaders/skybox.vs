
//INPUTS=======================================================================
layout (location = 0) in vec3 i_vertex;

//UNIFORMS=====================================================================
uniform mat4 u_viewProjectionMatrix;

//OUTPUTS======================================================================
out block{
	vec3 vertex;
}OUT;


//CODE=========================================================================
void main()
{	
	OUT.vertex = i_vertex;

	gl_Position = u_viewProjectionMatrix * vec4(i_vertex, 1.0);
}
