#version 330


in vec3 v_position;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform samplerCube tex_image;

out vec4 ex_texel;

void main(void) {
    gl_Position = proj_matrix * view_matrix* model_matrix * vec4(v_position, 1.0f);
    ex_texel = texture(tex_image, v_position);
}
