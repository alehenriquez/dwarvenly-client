#version 330

out vec4 frag_color;

in vec3 n_normal;
in vec3 v_to_light;


void main () {
    const vec4 ambient = vec4(0.1, 0.0, 0.0, 1.0);
    const vec4 diffuse = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 nn_normal = normalize(n_normal);
    vec3 nv_to_light = normalize(v_to_light);
    float diff = clamp(dot(n_normal, v_to_light), 0.0, 1.0);

    frag_color = ambient + diffuse + diff;
}
