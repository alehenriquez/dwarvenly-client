#version 330

out vec4 frag_color;

void main () {
    /*const vec4 ambient = vec4(0.8, 0.8, 0.5, 1.0);
    const vec4 diffuse = vec4(0.4, 0.1, 0.1, 1.0);

    vec3 nn_normal = normalize(n_normal);
    vec3 nv_to_light = normalize(v_to_light);
    float diff = clamp(dot(n_normal, v_to_light), 0.0, 1.0);
    */
    frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
