#version 330

uniform vec4 ambient_color;
uniform vec4 diffuse_color;
uniform vec4 specular_color;
uniform sampler2D colormap;
uniform sampler2D normalmap;

in vec3 light_direction;
in vec3 normal;
in vec2 texcoords;

out vec4 frag_color;

void main () {
	const float max_variance = 0.5; // Mess around with this value to increase/decrease normal perturbation
	const float min_variance = max_variance / 2.0;

	// Create a normal which is our standard normal + the normal map perturbation (which is going to be either positive or negative)
	vec3 normal2 = normalize(normal + normalize(texture(normalmap, texcoords).rgb * max_variance - min_variance));

	// Calculate diffuse intensity
	float diffuse_intensity = max(0.0, dot(normal2, light_direction));

	// Add the diffuse contribution blended with the standard texture lookup and add in the ambient light on top
	vec3 color = (diffuse_intensity * diffuse_color.rgb) * texture(colormap, texcoords).rgb + ambient_color.rgb;
	frag_color = vec4(color, 1.0);

	
	// Calc and apply specular contribution
	vec3 reflection = normalize(reflect(-normal2, light_direction));
	float specular_intensity = max(0.0, dot(normal2, reflection));
	frag_color.rgb += vec3(pow(specular_intensity, 64.0) * specular_color.rgb);
}
