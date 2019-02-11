
//INPUTS=======================================================================
layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec2 i_uv;

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;

//OUTPUTS======================================================================
out block{
	vec2 uv;
}OUT;


//CODE=========================================================================
void main()
{	
	OUT.uv = i_uv;
	
	gl_Position = u_modelViewProjectionMatrix * vec4(i_vertex, 1.0);
}
