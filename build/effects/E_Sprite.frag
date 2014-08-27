uniform sampler2D uTexDiffuse;
varying vec2 vTexCoord;
void main()
{
	vec4 pc = texture2D(uTexDiffuse,vTexCoord);
	gl_FragColor = pc;
}
