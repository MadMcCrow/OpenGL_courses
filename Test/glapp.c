/* basic gl related functions to free main.c to allow for more GAME-ISH things in main.c
 * Tourner autour d'un cube
 * gcc code004.c glutils.c glmath.c -o code004 -lGL -lGLEW -lglut -lm
 */
#include "glmath.h"
#ifndef STDLIB_INCLUDED
#define STDLIB_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutils.h"
#endif
#ifndef READFILES_H
#define READFILES_H
#include "readfiles.h"
#endif


void set_mvp( void )
{
	mat4 projection_matrix =
		mat4_perspective( 45., 1000. / 1000., .1, 100. );
	mat4 view_matrix =
		mat4_lookAt(app.mvp_camera,
			    app.mvp_lookat,
			    app.mvp_vertical
			    );

	app.mvp_matrix =
		mat4_product(
			mat4_product(projection_matrix, view_matrix),
			app.mvp_model
			);
}

/** Rafraichissement de la fenetre
 *
 * Lorsqu'il faut regenerer le contenu de la fenetre :
 * - evenement externe (par ex., une autre fenetre qui masquait
 *   une partie de la fenetre vient de disparaitre)
 * - evenement interne (par ex., le point de vue a change)
 */
void my_refresh( void )
{
	//glEnable( GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	// on efface la fenetre
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// on (re)met en place les bons shaders
	glUseProgram( app.program_id );
	// on pousse la matrice de point de vue sur la carte
	glUniformMatrix4fv(
		app.mvp_matrix_id,
		1,
		GL_FALSE,
		&app.mvp_matrix.matrix[0][0]
		);
	// on se branche sur la bonne source...
	glEnableVertexAttribArray( 0 );
	// ... et on enfonce la prise
	glBindBuffer( GL_ARRAY_BUFFER, app.vertex_buffer_id );
	// description de la structure des donnees pour la base
	glVertexAttribPointer(
		0, // cf. location in vertex shader
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
	// attributs de couleur
	glEnableVertexAttribArray( 1 );
	glBindBuffer(GL_ARRAY_BUFFER, app.color_buffer_id);
	glVertexAttribPointer(
		1, // cf. location in vertex shader
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
	// dessiner l'objet...
	glDrawArrays( GL_TRIANGLES, 6, 12 * 3 );
	// ... et dessiner la base
	glDrawArrays( GL_LINES, 0, 6 );
	// on debranche
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );
	// on valide l'affichage
	glutSwapBuffers();
}

/** gestion des événements clavier
 *
 * @param key code (caractere) de la touche
 * @param x coordonnee du pointeur au moment de l'action
 * @param y coordonnee du pointeur au moment de l'action
 * @param action 1 pour enfonce, 0 pour relache
 */
void my_keyboard( unsigned char key, int x, int y, int action )
{
	printf("keyboard event : <%d> @ %d %d (action=%d)\n",
	       key, x, y, action
	       );
	switch (key) {
	case 27:                        // <Escape>
		glutLeaveMainLoop();    // freeglut
		break;
	case 'a':
		if (action) {
			app.mvp_camera =
				vec3_transform(mat4_rotate(.1, app.mvp_vertical),
					       app.mvp_camera
					       );
			set_mvp();
			glutPostRedisplay();
			break;
		}
	}
}

/** touche clavier appuyee
 *
 * @param key code (caractere) de la touche
 * @param x coordonnee du pointeur au moment de l'action
 * @param y coordonnee du pointeur au moment de l'action
 */
void my_keyboard_press( unsigned char key, int x, int y )
{
	my_keyboard( key, x, y, 1 );
}

/** touche clavier relachee
 *
 * @param key code (caractere) de la touche
 * @param x coordonnee du pointeur au moment de l'action
 * @param y coordonnee du pointeur au moment de l'action
 */
void my_keyboard_release( unsigned char key, int x, int y )
{
	my_keyboard( key, x, y, 0 );
}

/** boucle principale
 */
void mainloop( void )
{
	app.state = RUN;

	glClearColor( 0.0f, 0.0f, 0.4f, 0.0f );

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutMainLoop();

	app.state = FINISH;
}

/** fin de programme
 *
 * Appele quand l'application se termine *normalement*,
 * c'est-a-dire s'il n'y a pas eu plantage grave ou appel a
 * la fonction abort().
 */
void bye( void )
{
	printf( "that's all folks! (state=%d)\n", app.state );
}

/** initialisation de l'application
 *
 * @param argc argument count (transmis par l'interface de main)
 * @param argv argument vector
 * @return 0 si tout est ok, 1 en cas de probleme
 *
 * glutInit peut egalement provoquer l'arret du programme en cas
 * de probleme : pas de retour dans main(), mais le callback bye
 * sera appele (cf. atexit en debut de main).
 */
int init_resources( int *argc, char **argv )
{
	// 1) Connexion au système graphique natif & initialisation
	glutInit( argc, argv );

	// 2) creation d'une fenetre
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( 800, 800 );
	app.window_id = glutCreateWindow( "My little space" );

	glutKeyboardFunc( my_keyboard_press );
	glutKeyboardUpFunc( my_keyboard_release );
	glutDisplayFunc( my_refresh );

	// 3) exposition de l'API (*apres* la creation d'une fenetre)
	glutInitContextVersion(3, 3); // a rajouter pour fonctionner sous iGP intel (besoin de spécifier la version OpenGL et de GLSL)
	glewExperimental =  GL_TRUE;
	if ( glewInit() != GLEW_OK )
		return 1;
	// 4) les shaders
	char* vertex_shader = NULL;
	char* fragment_shader = NULL;
	vertex_shader = read_file(vertex_shader, "shaders/shader1.glslv");
	fragment_shader = read_file(fragment_shader, "shaders/shader1.glslf");
	app.program_id = load_shaders( vertex_shader, fragment_shader );
	// pas besoin de garder les shaders en mémoire.
	free(vertex_shader);
	free(fragment_shader);
	//si ça n'a pas marché on se taille.
	if ( app.program_id == 0 ) return 1;

	// 5) le point de vue initial
	app.mvp_model = mat4_identity();
	app.mvp_matrix_id = glGetUniformLocation( app.program_id, "MVP" );
	app.mvp_camera = vec3_init( 4, 3, 3 );
	app.mvp_lookat = vec3_init( 0, 0, 0 );
	app.mvp_vertical = vec3_init( 0, 1, 0 );
	set_mvp();

	// 6) le modèle géométrique
	glGenVertexArrays( 1, &app.vertex_array_id );
	glBindVertexArray( app.vertex_array_id );

	char *obj_str = NULL;
	obj_str = read_file(obj_descr, "objects/cube.obj");
	// Count vertices
	int vert_count = 0;
	char* obj_ptr = obj_str;
	while (*obj_ptr) {
		vert_count += *(obj_ptr++) == 'v' ? 1 : 0;
	}
	// Allocate vertices
	vertex* verts = malloc(sizeof(vertex) * vert_count);
	parse_obj(*obj_str,*verts, vert_count);
	free(obj_descr);

	static const GLfloat g_vertex_buffer_data[] =
	{
		1.0, 0.0, 0.0, // base
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, // "modele 3D"
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, // "modele 3D"
		1.0, 1.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, // "modele 3D"
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 1.0, 1.0, // "modele 3D"
		1.0, 1.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, // "modele 3D"
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 1.0, // "modele 3D"
		0.0, 1.0, 0.0,
		1.0, 0.0, 1.0,



	};

	glGenBuffers( 1, &app.vertex_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, app.vertex_buffer_id );
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(g_vertex_buffer_data),
		g_vertex_buffer_data,
		GL_STATIC_DRAW
		);

	static const GLfloat g_color_buffer_data[] =
	{
		1, 0, 0,        // x (rouge)
		1, 0, 0,
		0, 1, 0,        // y (vert)
		0, 1, 0,
		0, 0, 1,        // z (bleu)
		0, 0, 1,
		1, 1, 0,        // objet (YCM)
		0, 1, 1,
		1, 0, 1,
		1, 0, 0, // objet (YCM)
		1, 0, 0,
		1, 0, 0,
		1, 1, 0, // objet (YCM)
		0, 1, 1,
		1, 0, 1,
		1, 1, 0, // objet (YCM)
		0, 1, 1,
		1, 0, 1,
		1, 1, 0, // objet (YCM)
		0, 1, 1,
		1, 0, 1,
		1, 1, 0, // objet (YCM)
		0, 1, 1,
		1, 0, 1,

	};

	glGenBuffers( 1, &app.color_buffer_id );
	glBindBuffer( GL_ARRAY_BUFFER, app.color_buffer_id );
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(g_color_buffer_data),
		g_color_buffer_data,
		GL_STATIC_DRAW
		);

	return 0;
}

/** nettoyage de l'application
 */
void free_resources( void )
{
	glDeleteBuffers(1, &app.vertex_buffer_id);
	glDeleteProgram(app.program_id);
	glDeleteVertexArrays(1, &app.vertex_array_id);
}




int parse_obj(char *str, vertex *verts, int vert_count) {
	
	// Read the vertices
	int cur   = 0;
	int cur_t = 0;
	int cur_n = 0;
	char* ptr = str;
	while (*ptr) {
		// Skip spaces
		while (isspace(*ptr)) ptr++;
		// Skip comments
		if (*ptr == '#') {
			while (*ptr != '\n' && *ptr != 0) ptr++;// There might be some space after that comment, we need to skip it as well.
			continue;
		}
		// At this point, we have a character that is not space nor a comment
		if (*ptr == 'v' && isspace(ptr[1])) {
			// We have a vertex, but it might be broken... Beware!
			ptr++;
			for (int i = 0; i < 3 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				verts[cur].position.row[i] = strtof(ptr, &ptr);
			}
			cur++;			
		}
		else if ( ptr[0] == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
		// we have a vertex uv vector
		ptr++;
			for (int i = 0; i < 2 && *ptr; i++) {
				verts[cur].uv.row[i] = strtof(ptr, &ptr);
			}
			cur_t++;
		}else if ( ptr[0] == 'v' && ptr[1] == 'n' && isspace(ptr[2])) {
		// we have a vertex normal
		ptr++;
			for (int i = 0; i < 3 && *ptr; i++) {
								verts[cur].normal.row[i] = strtof(ptr, &ptr);
			}
			cur_n++;
		}else {
		// Something else. pass.
			*ptr++;
		}
	}
	assert(cur == vert_count);
	//assert(cur == cur_n);
	return 0;
}

