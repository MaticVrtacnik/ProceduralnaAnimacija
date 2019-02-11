
//UNIFORMS=====================================================================
uniform vec3 u_color;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{	
	o_fragColor = vec4(u_color, 1.0);
}
