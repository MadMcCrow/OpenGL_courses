typedef int face[3];

_Bool Gen_checker_backend(int tx, int ty, GLfloat *vertex, GLuint *indices);

int Gen_checker(int x, int y, GLfloat *vertex_buffer_data, GLuint *indices, _Bool axes);
