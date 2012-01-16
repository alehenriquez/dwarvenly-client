#version 330

uniform Transformation {
    mat4 proj_matrix;
    mat4 model_matrix;
    mat4 view_matrix;
};

in vec3 v_position;
in vec3 v_normal;


void main(void) {
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(v_position, 1.0f);
}
