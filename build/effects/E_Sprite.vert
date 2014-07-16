uniform mat3 ProjectionMatrix;
uniform mat3 ViewMatrix;
uniform mat3 ModelMatrix;
varying vec2 texCoord;
void main()
{
	gl_Position =   ProjectionMatrix*ViewMatrix*ModelMatrix*gl_Vertex;
	texCoord = gl_MultiTexCoord0.xy;
}
