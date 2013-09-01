#version 330

in vec4 diffuse;
in vec4 ambient;

out vec4 frag_color;

void main () {
	// Use the diffuse contribution and add in the ambient light on top
	vec3 color = diffuse.rgb + ambient.rgb;
	frag_color = vec4(color, diffuse.a);
}
