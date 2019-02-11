
//OUTPUTS======================================================================
layout (location = 0) out vec2 o_fragColor;


//CODE=========================================================================
void main()
{	
	float moment1 = gl_FragCoord.z;
	float dx = dFdx(moment1);
	float dy = dFdy(moment1);
	
	float moment2 = moment1 * moment1 + 0.25 * (dx * dx + dy * dy);

	o_fragColor = vec2(moment1, moment2);
}
