#version 130
uniform mat3 uProjectionMatrix;
uniform mat3 uViewMatrix;
uniform mat3 uModelMatrix;
in vec2 aPosition;
in vec2 aTexCoord;
varying vec2 vTexCoord;
void main()
{
	vec3 projected = uProjectionMatrix*uViewMatrix*uModelMatrix*vec3(aPosition.xy,1.0);
	gl_Position =  vec4(projected.xy,0.0,1.0);
	vTexCoord = aTexCoord.xy;
}
