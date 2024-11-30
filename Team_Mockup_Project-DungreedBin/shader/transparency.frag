uniform sampler2D texture;
uniform float trans_alpha;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	pixel.a *= trans_alpha;
	gl_FragColor = pixel;
}
