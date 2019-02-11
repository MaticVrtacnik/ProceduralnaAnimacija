
//UNIFORMS=====================================================================
uniform vec4 u_color;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out vec4 o_bloomColor;

//CODE=========================================================================
void main()
{
	o_fragColor = u_color;
	o_bloomColor = vec4(0.0, 0.0, 0.0, 0.0);
}
