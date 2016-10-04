
#include <stdio.h>
#include <stdlib.h>


/* GLFW version 
#include <GLFW/glfw3.h>

int init_ressources(void){return 1;}
void my_close(GLFWwindow *window)
{
int *pmore =glfwGetWindowUserPointer(window);
if (pmore)
{printf("")} 
}
void goodbye (void)
{
printf("exit");
}

int main (int argc, char *argv)
{


printf("going home")
return 0;
}

*/

#include <GL/freeglut.h>

int init_ressources(void){return 1;}
void free_ressources(){}
//keyboard inputs 
void my_keyboard(unsigned char key, int x, int y, int action)
{       printf("keyboard event: <%d> @ %d %d (action= %d)\n", key, x, y, action);
        switch(key)
        {
                case 27:
                glutLeaveMainLoop();
                break;
        }
}

void my_keyboard_press (unsigned char key, int x, int y)
{
        my_keyboard(key,x,y,0);
}
void my_keyboard_release (unsigned char key, int x, int y)
{
        my_keyboard(key,x,y,1);
}

void display(void)
{
glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
//fuck you
glutSwapBuffers();
glutPostRedisplay ();
}

// end of program

void bye(void)
{
printf("have a good day !/n");
}

//main program
int main (int argc, char **argv)
{
atexit(bye);
glutInit (&argc, argv);
// pile of settings
glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
glutInitWindowPosition(0,0);
glutInitWindowSize(800,800);
glutCreateWindow("My little Pony is a terrible show");
glutDisplayFunc(display);
glutKeyboardFunc(my_keyboard_press);
glutKeyboardUpFunc(my_keyboard_release);


//lets dance mothaFuckers
if (!init_ressources())
{
        fprintf(stderr, "error. something went wrong during init ... you suck.\n");
        return 1;
}

glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS );
glutMainLoop();
free_ressources();
return 0;

}

