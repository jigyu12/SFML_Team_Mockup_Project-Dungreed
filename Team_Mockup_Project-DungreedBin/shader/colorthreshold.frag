uniform sampler2D texture;
uniform float th_g;
uniform float th_b;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	if(pixel.r>0.5)
	{
		pixel.g += th_g;
		pixel.b += th_b;
	}
	gl_FragColor = vec4(pixel.r,pixel.g,pixel.b, pixel.a);
}
