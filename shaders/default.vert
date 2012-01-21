#version 330


in vec3 v_position;
in vec3 v_color;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

out vec3 ex_color;


void main(void) {
    gl_Position = view_matrix * model_matrix *vec4(v_position, 1.0f);
    ex_color = v_color;
}
