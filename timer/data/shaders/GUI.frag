
//INPUTS=======================================================================
in block{
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_texture;

//UNIFORMS=====================================================================
uniform vec4 u_color;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{
	o_fragColor = texture(u_texture, IN.uv) + u_color;
}
