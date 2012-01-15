//= preamble
uniform sampler2D tex0;

//= body
uniform float strength;

vec3 desaturate(const vec3 color, const float desaturation)
{
	const vec3 gray_conv = vec3(0.30, 0.59, 0.11);
	vec3 gray = vec3(dot(gray_conv, color));
	return vec3(mix(color.rgb, gray, desaturation));
}

//= main
void main(void) {
	//= main-preamble
	vec4 color = gl_Color * texture2D(tex0, gl_TexCoord[0].xy);

	//= main-body
	color.rgb = desaturate(color.rgb, strength);

	//= main-postscript
	gl_FragColor = color;

	//= main-end
}