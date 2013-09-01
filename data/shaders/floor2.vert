#version 330

in vec3 v_position;
in vec4 v_diffuse;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform vec4 ambient_color;

out vec4 diffuse;
out vec4 ambient;

void main(void) {
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(v_position, 1.0f);
    diffuse = v_diffuse;
    ambient = ambient_color;
}
