uniform sampler2D TexDiffuse;
varying vec2 texCoord;
void main()
{
	vec4 pc = texture2D(TexDiffuse,texCoord);
	gl_FragColor = pc;
}
