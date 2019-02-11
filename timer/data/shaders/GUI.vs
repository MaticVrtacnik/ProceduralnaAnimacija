
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
