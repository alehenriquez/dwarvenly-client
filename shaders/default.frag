#version 330

uniform samplerCube tex_image;

in vec3 ex_texcoords;
in vec4 ex_light_direction;
in vec3 ex_normal;

out vec4 frag_color;

void main () {
    //const vec4 ambient = vec4(0.8, 0.8, 0.5, 1.0);
    //const vec4 diffuse = vec4(0.4, 0.1, 0.1, 1.0);
    vec4 texel = texture(tex_image, ex_texcoords);
    float diffuse = clamp(dot(vec4(ex_normal, 1.0), ex_light_direction), 0.0, 1.0);

    frag_color = diffuse+texel;
}
