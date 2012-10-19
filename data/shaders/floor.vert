#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoords;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform vec3 light_pos;

out vec3 light_direction;
out vec3 normal;
out vec2 texcoords;

void main(void) {
    gl_Position = proj_matrix * view_matrix* model_matrix * vec4(v_position, 1.0f);
    texcoords = v_texcoords;
    light_direction = normalize(light_pos - gl_Position.xyz); // vec4(1.0, -1.0, 0.0, 1.0)
    normal = normalize(v_normal);
}
