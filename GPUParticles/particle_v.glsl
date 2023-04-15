#version 330

layout(location=0) in float Count;

uniform sampler2D Vertex;
uniform sampler2D PColor;

uniform int ParticleSysSize;

out vec4 Color;

void main()
{
	// Use the current vertex count as an index into the square texture, x wraps around, y is scaled.
	// Could use gl_VertexID too, but might as well make use of the useless VBO.
	ivec2 Coords=ivec2(gl_VertexID%ParticleSysSize, gl_VertexID/ParticleSysSize);
	vec4 Pos=texelFetch(Vertex, Coords, 0);
	vec4 PColor=texelFetch(PColor, Coords, 0);

	// Feed the texture data directly into the geometery shader.
	gl_Position=vec4(Pos.xyz, 1.0);

	// Color is just a simple mix using the particle "life", also passes life as alpha color (for fade).
	Color=vec4(mix(vec3(0.3, 0.3, 0.3), PColor.xyz, Pos.w+0.3), Pos.w);
}
