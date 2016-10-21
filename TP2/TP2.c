#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Glunint compile_shader(const char * shader_source, Glenum shader_type)
{
Gluint shader_id = glCreateShader(shader_type);
//previens la CG d'un nouveau shader
glShaderSource(shader_id, 1, &shader_source, NULL);
//charge coté OS  (dans le driver) le code source 
glCompileShader(shader_id);
//compile le shader
GLint result=GL_FALSE;
glGetShaderiv;
glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    int log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        char *log_message = malloc(log_length+1);
        if (log_message) {
            glGetShaderInfoLog(shader_id, log_length, NULL, log_message);
            printf("%s\n",log_message);
            free(log_message);
        }
        else
            printf("unable to display compile log message\n");
    }
    if (!result) {
        glDeleteShader(shader_id);
        return result ? shader_id : 0;
    }
    
}


//loadshader
Gluint load_shaders (const char * vertex_shader_source,
    const char * fragment_shader_source)
{
//compile un vertex shader
GLuint vertex_shader_id = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
	//si on échoue on abandonne
	if (vertex_shader_id == 0) {
        return 0;
    }
//compile un fragment shader_id
    GLuint fragment_shader_id= compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    // si echec on rejete tout
    if (fragment_shader_id == 0) {
        glDeleteShader(vertex_shader_id);
        return 0;
    }
	
//link les 2 shaders pour faire un programme
GLuint program_id = glCreateProgram ();
//(on assemble les shaders)
glAttachShader(program_id, vertex_shader_id)
glAttachShader(program_id, fragment_shader_id);
//glAttachShader(program_id, ...) si il y en a plus.
glLinkProgram(program_id);
	//vérifie qu'il n'y a pas de problème
GLuint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    int log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length>0) {
        char *log_message = malloc(log_length+1);
        if (log_message) {
            glGetProgramInfoLog(program_id, log_length, NULL, log_message);
            printf("%s\n", log_message);
        }
        else
            printf("unable to display link log message");
    }
    
    // se débarasse des shaders.
    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    
    if(!result) glDeleteProgram(program_id);
    return result ? program_id : 0;
}
//-------------------------------------------------------------------------------
typedef struct
{
    int more;
    GLFWwindow* window;
    GLuint program_id;
    GLuint vertex_buffer;
} Application;

int init_ressources(void){return 1;}
// fonction de rappel (requêtes de fermeture)
void my_close(GLFWwindow *window)
{
    Application *a = glfwGetWindowUserPointer(window);
    if ((a->more=0)) {
        printf("fermeture\n");
        a->more=0;
    }
}

//fonction de rappel (événements clavier)
void my_keyboard(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mods
){
    // pour éviter un "warning: unused parameter" à la compilation
    (void) scancode;
    
    printf("keyboard event : <%d> (action=%x, mods=%x)\n",key,action,mods);
    switch (key) {
        case GLFW_KEY_ESCAPE:
            {
                Application *a = glfwGetWindowUserPointer(window);
                if (a->more) a->more=0;
            }
            break;
    }
}




// fonction de dessin (isolée pour la lisibilité)
void display(
Application *a
)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //utilisation des shaders identifiés
    glUseProgram(a->program_id);
    //active l'accès à l'attribut 0 de la source courante
    glBindBuffer(GL_ARRAY_BUFFER, a->vertex_buffer);
    glVertexAttribPointer(
                          0,            //attribue 0
                          3,            //taille
                          GL_FLOAT,     //type
                          GL_FALSE,     //normalisé
                          0,            //entrelacement
                          (void*)0      //décalage
                          );    //connecte le VBO courant au VAO
    // 3 sommets, partir du n.0 (1 triangle)
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}



void my_display(
                GLFWwindow* window
)
{
    Application *a = glfwGetWindowUserPointer(window);
    display(a);
    glfwSwapBuffers(window);
}

//programme principal
int main(void)
{
    // 1) connexion au système graphique natif & initialisation
    if (!glfwInit()) {
        return 1;
    }
    // 2) creation d'une fenetre
    // version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //*seulement* 3.3 ou plus
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        GLFWwindow* window =
        glfwCreateWindow(800,800, "My little space", NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
            return 1;
        }
        glfwSetWindowCloseCallback(window, my_close);
        glfwSetKeyCallback(window, my_keyboard);
        // pour (re)dessiner quand nécéssaire
        glfwSetWindowRefreshCallback(window, my_display);
        
    // 3) let's rock!
        glfwMakeContextCurrent(window);
        //exposera les extensions non présentes dans le driver
    glewExperimental = 1;
    //expose les extensions
    if (glewInit()!= GLEW_OK) {
        glfwTerminate();
        return 1;
    }
    
    Application application;
    
    static char vertex_shader[]=
        "#version 330 core\n"
        "layout(location = 0) in vec3 vertexPosition_modelspace;"
        "void main()"
        "{"
        "   gl_Position.xyz = vertexPosition_modelspace;"
        "   gl_Position.w = 1.0;"
        "}";
    char fragment_shader[] =
        "#version 330 core\n"
        "out vec3 color;"
        "void main()"
        "{"
        "   color = vec3(1,0,0);"
        "}";
    application.program_id = load_shaders(vertex_shader, fragment_shader);
    if (application.program_id==0) {
        glfwTerminate();
        return 1;
    }
    
    GLuint vao_id; // VAO = porte d'entree du pipe-line
    glGenVertexArrays(1, &vao_id); // création
    glBindVertexArray(vao_id); // attachement au contexte courant
    
    static const GLfloat vertices[] =
    {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        0.0, 1.0, 0.0
    };
    // stockage des vertices
    glGenBuffers(1, &application.vertex_buffer);
    // attachement au contexte courant
    glBindBuffer(GL_ARRAY_BUFFER, application.vertex_buffer);
    // allocation mem et copie des données
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    application.more=1;
    glfwSetWindowUserPointer(window, &application);
    while (application.more) {
        glfwWaitEvents();
    }
    glfwTerminate();
    //that's all folks!
    printf("That's all folks!\n");
    return 0;
    }
    
    
    /*




//shader: 

//vertex shader-> writen in GLSL
char vertex_shader [] = 
"#version 330 core\n"
// location 0 choisi arbitrairement -> ou attribué par Glquelquechose parce que les autres numéros sont utilisé
"layout(location = 0) in vec3 vertexPosition_modelspace;" //vec3 type
"void main()"
"{"
"gl_Position.xyz = vertexPosition;"
"gl_Position.w = 1.0;"
"}";
char Fragment_shader [] = 
"#version 330 core\n"
"out vec3 color;"
"void main()"
"{"
"color= vec3(1,0,0);"
"}";
application.program_id = loadshaders (vertex)
if (application.program_id == 0)
{
glfwTerminate();
return 1;
}
Gluint vao_id; //VAO porte d'entrée du pipeline : Vertex Array Object != VPO : Vertex Buffer Object
glGenVertexArrays(1,&vao_id); //création du VAO.
glBindVertexArray(vao_id); //attachement au contexte courant.
static const GLfloat vertices[] =
{
-1.0, -1.0 , 0.0,
 1.0, -1.0 , 0.0,
 0.0,  1.0 , 0.0,
};
//stockage des vertices
glGenBuffers(1, &application.vertex_buffer);
glBindBuffer(GL_ARRAY_BUFFER, application.vertex_buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

application.more = 1;
glfwSetWindowUserPointer(window,&application);
while (application.more)
{
glfwWaitEvents();
}

*/
