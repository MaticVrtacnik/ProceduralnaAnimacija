
layout (location = 0) in vec3 vertex;

noperspective out vec2 outUV;

void main()
{
	gl_Position = vec4(vertex, 1.0);
	outUV = vertex.xy * 0.5 + 0.5;
}
