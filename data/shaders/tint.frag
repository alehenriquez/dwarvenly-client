//= preamble
uniform sampler2D tex0;

//= body
uniform vec4 tint = vec4(1.0, 0.0, 0.0, 1.0);

//= main
void main(void) {
	//= main-preamble
	vec4 color = gl_Color * texture2D(tex0, gl_TexCoord[0].xy);

	//= main-body
	color *= tint;

	//= main-postscript
	gl_FragColor = color;

	//= main-end
}