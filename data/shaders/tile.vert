#version 330

in vec3 v_position;
in vec3 v_normal;
in vec3 v_texcoords;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform sampler2D tex_image;

out vec4 ex_light_direction;
out vec2 ex_texcoords;
out vec3 ex_normal;

void main(void) {
    gl_Position = proj_matrix * view_matrix* model_matrix * vec4(v_position, 1.0f);
    ex_texcoords = v_texcoords;
    ex_light_direction = normalize(vec4(1.0, -1.0, 0.0, 1.0));
    ex_normal = normalize(v_normal);
}
