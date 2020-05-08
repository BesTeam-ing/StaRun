#include "header.h"

SkyBox sky;

void initGlobals(void)
{
    /* WINDOW */
    windowHeight=DEF_WINDOW_HEIGHT;
    windowWidth=DEF_WINDOW_WIDTH;
    windowPosHeight=DEF_WINDOW_POS_H;
    windowPosWidth=DEF_WINDOW_POS_W;

    /* PROJECTION */
    dim=DEF_DIM;
    th=DEF_TH;
    ph=DEF_PH;
    fov=DEF_FOV;
    asp=DEF_ASP;
    ecX=DEF_ECX;
    ecY=DEF_ECY;
    ecZ=DEF_ECZ;
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glViewport(0, 0, 1280, 720);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1280, 0.0f, 720, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glutReshapeWindow(1280, 720);
}

void windowKey(unsigned char key,int x,int y)
{
  /*  Exit on ESC */
  if (key == 27)
      exit(0);
  /*  Change field of view angle */
  else if (key == '-' && key>1)
      fov--;
  else if (key == '+' && key<179)
      fov++;

  redisplayAll();
}

void windowSpecial(int key,int x,int y)
{
  /*  Right/Left - rotate */
    if (key == GLUT_KEY_RIGHT) th += 5;
    else if (key == GLUT_KEY_LEFT) th -= 5;
  /*  Up/Down - elevation */
    else if (key == GLUT_KEY_UP) ph += 5;
    else if (key == GLUT_KEY_DOWN) ph -= 5;
  
  /*  Keep angles at +/- 360 degrees */
  th %= 360;
  ph %= 360;

  redisplayAll();
}

void drawScene()
{
    /* setup functions */
    displayInit();
    displayEye();

    sky.drawSkyBox(3.5*dim);

    /* Flush, SwapBuffers, and sanity check */
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    initGlobals();

    /* screencast specific variables */
    screencastID = 21;
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(windowPosWidth,windowPosHeight);
    glutCreateWindow(windowName);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(displayReshape);
    glutKeyboardFunc(windowKey);
    glutSpecialFunc(windowSpecial);

    sky.initSkyBox("textures/txStormydays_front.bmp", "textures/txStormydays_right.bmp", "textures/txStormydays_left.bmp", "textures/txStormydays_back.bmp", "textures/txStormydays_up.bmp", "textures/txStormydays_down.bmp");

    redisplayAll();
    glutMainLoop();
    return 0;
}

