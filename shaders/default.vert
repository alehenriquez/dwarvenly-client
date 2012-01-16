#version 330

in mat4 proj_matrix;
in mat4 model_matrix;
in mat4 view_matrix;

in vec3 v_position;
in vec3 v_normal;

out vec3 position;
out vec3 normal;
out vec3 v_to_light;
out mat4 view_model;

void main(void) {
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(v_position, 1.0);
    position = vec3(view_matrix * model_matrix * vec4(v_position, 1.0));
    normal = normalize(vec3(view_matrix * model_matrix * vec4(v_normal, 0.0)));
    view_model = view_matrix * model_matrix;
    v_to_light = vec3(vec3(0.0, 20.0, 0.0)-vec3(gl_Position));
}
