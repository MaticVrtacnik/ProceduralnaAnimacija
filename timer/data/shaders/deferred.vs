
//INPUTS=======================================================================
layout (location = 0) in vec4 i_vertexColorR;
layout (location = 1) in vec4 i_normalColorG;
layout (location = 2) in vec4 i_tangentColorB;
layout (location = 3) in vec4 i_uv;

//UNIFORMS=====================================================================
uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform mat4 u_prevModelViewProjectionMatrix;
uniform mat3 u_normalMatrix;
uniform vec3 u_color;


//OUTPUTS======================================================================
out block{
	vec4 velocityVertex;
	vec4 prevVelocityVertex;
	vec3 vertex;
	vec3 normal;
	vec3 tangent;
	vec3 color;
	vec2 uv;
}OUT;


//CODE=========================================================================
void main()
{
	vec4 viewVertex = u_modelViewMatrix * vec4(i_vertexColorR.xyz, 1.0);
	
	OUT.velocityVertex = u_projectionMatrix * viewVertex;
	OUT.prevVelocityVertex = u_prevModelViewProjectionMatrix * vec4(i_vertexColorR.xyz, 1.0);
	OUT.vertex = viewVertex.xyz;
	OUT.tangent = u_normalMatrix * i_tangentColorB.xyz;
	OUT.normal = u_normalMatrix * i_normalColorG.xyz;
	//OUT.normal = u_normalMatrix * vec3(0.0, 1.0, 0.0);
	//OUT.tangent = OUT.tangent - dot(OUT.tangent, OUT.normal) * OUT.normal;	
	OUT.uv = i_uv.xy;
	
	OUT.color = vec3(i_vertexColorR.a, i_normalColorG.a, i_tangentColorB.a);
	OUT.color = u_color;
	
	gl_Position = OUT.velocityVertex;
}
