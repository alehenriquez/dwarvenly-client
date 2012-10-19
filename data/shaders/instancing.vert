//= preamble
attribute mat4 transform_matrix;

//= body

//= main
void main(void) {
    //= main-preamble
    mat4 mvp = gl_ModelViewProjectionMatrix * transform_matrix;

    //= main-body

    //= main-postscript
    gl_Position = mvp * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    //= main-end
}