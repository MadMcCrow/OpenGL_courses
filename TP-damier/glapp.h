/* basic gl related functions to free main.c to allow for more GAME-ISH things in main.c

/** Structure de donnees regroupant toutes les valeurs importantes.
 */
 typedef struct {
	vec3 position;
	vec2 uv;
	vec3 normal;
}vertex;

 
struct Application {
	// etat de l'application
	enum { INIT=0, RUN, FINISH } state;
	// identificateur de la fenetre de dessin
	int window_id;
	// identificateur du groupe de shaders
	GLuint program_id;
	// identificateur du descripteur de modele 3D
	GLuint vertex_array_id;
	// identificateur du buffer de modele 3D
	GLuint vertex_buffer_id;
	// identificateur du buffer de teintes
	GLuint color_buffer_id;
	// position de la camera
	vec3 mvp_camera;
	// position du point vise
	vec3 mvp_lookat;
	// reference verticale
	vec3 mvp_vertical;
	// matrice de visualisation
	mat4 mvp_matrix;
	// matrice de transformation
	mat4 mvp_model;
	// connexion avec le vertex shader (point de vue)
	GLuint mvp_matrix_id;
} app = { 0 };


void set_mvp( void );
void my_refresh( void );
void my_keyboard( unsigned char key, int x, int y, int action );
void my_keyboard_press( unsigned char key, int x, int y );
void my_keyboard_release( unsigned char key, int x, int y );
void mainloop( void );
void bye( void );
int init_resources( int *argc, char **argv );


