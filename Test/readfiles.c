/*------------------------------------------------------------------------------------------/
| these functions are made to help read files, as the shader or .obj files.
| they also make the conversion toward OpenGL-esque data that can be injected into main.c
|
|
| this code source has been only written by Perard-Gayot Noé.
| there is no licence per-se (So basically GPL3 -AFAIK),
| but I would really like to be mentionned as the original writer, 
| also please notify me if my work helped you (MadMcCrow on GitHub)
|-------------------------------------------------------------------------------------------*/


// this is merely to make the coe easier, some sort of alias.
FILE* open_readonly (char *filename){
FILE* fp = NULL;

return fp;
}
int file_size (FILE *fp){}

void generate_shaders (char *shader, size)
{
char *shader = malloc(size * sizeof(char));
}


void read_shader (char *vertex_shader, char *fragment_shader)