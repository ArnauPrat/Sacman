uniform mat3 ProjectionMatrix;
uniform mat3 ViewMatrix;
uniform mat3 ModelMatrix;
void main()
{
	vec3 projected = ProjectionMatrix*ViewMatrix*ModelMatrix*vec3(gl_Vertex.xy,1.0);
	gl_Position =  vec4(projected.xy,0.0,1.0);
}
