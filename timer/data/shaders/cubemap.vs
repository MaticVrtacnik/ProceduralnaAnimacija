
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
