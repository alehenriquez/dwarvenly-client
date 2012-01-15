#version 330

uniform Transformation {
    mat4 proj_matrix;
    mat4 model_matrix;
    mat4 view_matrix;
};

in vec3 v_position;

in vec3 v_normal;

out vec3 n_normal;
out vec3 v_to_light;

void main(void) {
    n_normal = vec3(view_matrix*model_matrix*vec4(v_normal, 1.0));
    gl_Position = proj_matrix*view_matrix*model_matrix*vec4(v_position, 1.0);
    v_to_light = vec3(vec3(0.0, 20.0, 0.0)-vec3(gl_Position));
}
