//= preamble
uniform sampler2D tex0;

//= body
uniform float scanline_pitch = 1.0;

//= main
void main(void) {
	//= main-preamble
	vec4 color = gl_Color * texture2D(tex0, gl_TexCoord[0].xy);

	//= main-body
	{
		int y = int(gl_FragCoord.y / scanline_pitch);
		if (y / 2 * 2 == y)
			color.a *= 0.1;
	}

	//= main-postscript
	gl_FragColor = color;

	//= main-end
}