//
//  Game.cpp
//  CandyRun
//
//  Created by Ciro De Vita on 12/05/2020.
//  Copyright © 2020 BesTeam-ing. All rights reserved.
//

#include "Game.hpp"

#define DEF_WINDOW_HEIGHT 720
#define DEF_WINDOW_WIDTH 1280
#define DEF_WINDOW_POS_H 50
#define DEF_WINDOW_POS_W 50

SkyBox sky;
WeatherEffects part;
Camera camera;
Road road;
Object obj;
Character character;
Menu menu;
GUI gui;
irrklang::ISoundEngine* engine;

GLdouble asp;
GLdouble dim = 15.0f;
GLdouble fov = 60.0f;

bool isStart = false;
bool isPaused = false;
bool isAudio = false;
bool isGameOver = false;
bool isDay = true;

enum weather{
    CLEAR,
    RAIN,
    SNOW
};
int weather_condition = CLEAR;

char* score;
char* vite;

float update = 0.1;
float speed = 0.1;

int background_chosen = 0;


Game::Game(int argc, char **argv, const char *name){
    this->argc = argc;
    this->argv = argv;
    this->windowName = name;
}

Game::~Game(){};

void Game::Timer(int value)
{
    if(isStart && !isPaused){
        if(update <= 1.5){
            if(10%(int(update*10)) == 0){
                std::cout<<"QUI: "<<update*10<<std::endl;
                speed = update;
                std::cout<<"Velocità: "<<speed<<std::endl;
            }
        }
        else
            return;
        
        update += 0.1;
        glutPostRedisplay();
    }
    glutTimerFunc(10000, Timer, 0);
}

void Game::init(){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT);
    glutInitWindowPosition(DEF_WINDOW_POS_W, DEF_WINDOW_POS_H);
    glutCreateWindow(this->windowName);
    
    glutDisplayFunc(this->drawScene);
    glutReshapeFunc(this->displayReshape);
    glutKeyboardFunc(this->windowKey);
    glutSpecialFunc(this->windowSpecial);
    glutMouseFunc(this->mouseInput);
    
    this->initAll();
        
    this->Timer(0);
    glutMainLoop();
}

void Game::initAll(){
    glClearColor(0.0,0.0,0.0,0.0);
    
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    
    camera.init(0.0f, 3.0f, 20.0f, 0.0, 2.0f, -1.0f, 0.0f, 1.0f, 0.0f);
    
    menu.initMenu();
        
    obj.load();
    obj.initObject();
    road.initializeGround();
                
    engine = irrklang::createIrrKlangDevice();
    engine->play2D("sounds/starwars.wav", true);
    engine->setSoundVolume(0.0f);
    
}

//float lightPos[] = { 0.0, 15.0, -9.0, 1.0 }; // Spotlight position.
//float spotDirection[] = {0.0, -1.0,0.0}; // Spotlight direction.

void Game::drawGame(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
    glLoadIdentity();
    
    camera.look();
    sky.drawSkyBox(9.5*dim);
    
    if(isPaused){
        glDisable(GL_LIGHTING);
        
        const char* text;
        const char* text_resume;
        const char* text_esc;
        int x, x1;
        if(isGameOver){
            text = "GAME OVER";
            text_resume = "PRESS 'P' TO RESTART";
            text_esc = "PRESS 'ESC' TO QUIT";
            x = -20;
            x1 = -15;
        }
            
        else{
            text = "PAUSE";
            text_resume = "PRESS 'P' TO RESUME";
            text_esc = "PRESS 'ESC' TO QUIT";
            x = -10;
            x1 = -15;
        }
        
        glPushMatrix();
            glColor4f(1.0, 1.0, 1.0, 0.0);
            glTranslatef(x, 3, 0);
            glScalef(0.05,0.05,0.05);
            for( const char* p = text; *p; p++){
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
            }
        glPopMatrix();
        
        glPushMatrix();
            glColor4f(1.0, 1.0, 1.0, 0.0);
            glTranslatef(x1, -2, 0);
            glScalef(0.02,0.02,0.02);
            for( const char* p = text_resume; *p; p++){
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
            }
        glPopMatrix();
        
        glPushMatrix();
            glColor4f(1.0, 1.0, 1.0, 0.0);
            glTranslatef(x1, -7, 0);
            glScalef(0.02,0.02,0.02);
            for( const char* p = text_esc; *p; p++){
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
            }
        glPopMatrix();
    }
    else{
        isGameOver = false;
                
        //WEATHER CONDITION
        switch (weather_condition) {
            case CLEAR:
                break;
            case RAIN:
                part.drawRain();
                break;
            case SNOW:
                part.drawSnow();
                break;
                
            default:
                break;
        }
        
        glEnable(GL_LIGHTING);
        
        
       

         /*
        glPushMatrix();
            glTranslatef(0, 2, 0);
            glutSolidSphere(2, 20, 20);
        glPopMatrix();
        */
        
        road.drawRoad(speed);
        
        gui.draw(character.getScore(), character.getLife());
        
        character.drawCharacter();
    
        obj.drawObject(speed, character.getX());
        
        if(obj.handleCollision(character.getX(), character.getY(), character.getZ()) == 1){
            std::cout<<"Collision"<<std::endl;
            character.setLife(-1);
            if(character.getLife() == 0)
                gameOver();
        }
        else if(obj.handleCollision(character.getX(), character.getY(), character.getZ()) == 2){
            std::cout<<"Premio +1"<<std::endl;
            character.setScore(10);
            engine->play2D("sounds/score.wav");
        }
}
    
    glFlush();
    glutSwapBuffers();
}

void Game::drawScene(){
    if(isStart)
        drawGame();
    else
        menu.drawMenu();
    
    redisplayAll();
}

void Game::gameOver(){
    engine->play2D("sounds/game_over.wav");
    obj.initObject();
    character.SaveHighScore();
    character.initialPosition();
    std::cout<<"Game Over"<<std::endl;
    isPaused = true;
    isGameOver = true;
    speed = 0.1;
    update = 0.1;
}

void Game::mouseInput(GLint button, GLint state, GLint x, GLint y){
    if(!isStart){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            int select = menu.select(x,y);
            
            if(select == 1){
                isStart = true;
                part.initParticles();
                std::cout<<"Caso :" <<menu.getBackground()<<std::endl;
                switch (menu.getBackground()) {
                    case 0: //GIORNO SERENO
                        sky.initSkyBox("textures/sky191ft.bmp", "textures/sky191rt.bmp", "textures/sky191lf.bmp", "textures/sky191bk.bmp", "textures/sky191up.bmp", "textures/sky191dn.bmp");
                        weather_condition = CLEAR;
                        isDay = true;
                        break;
                    
                    case 1: //GIORNO PIOGGIA
                        sky.initSkyBox("textures/sky303ft.bmp", "textures/sky303rt.bmp", "textures/sky303lf.bmp", "textures/sky303bk.bmp", "textures/sky303up.bmp", "textures/sky303dn.bmp");
                        weather_condition = RAIN;
                        isDay = true;
                        break;
                    case 2: //GIORNO NEVE
                        sky.initSkyBox("textures/sky303ft.bmp", "textures/sky303rt.bmp", "textures/sky303lf.bmp", "textures/sky303bk.bmp", "textures/sky303up.bmp", "textures/sky303dn.bmp");
                        weather_condition = SNOW;
                        isDay = true;
                        break;
                    
                    case 4: //MARTE
                        sky.initSkyBox("textures/pinkft.bmp", "textures/pinkrt.bmp", "textures/pinklf.bmp", "textures/pinkbk.bmp", "textures/pinkup.bmp", "textures/pinkdn.bmp");
                        weather_condition = CLEAR;
                        isDay = true;
                        break;
                        
                    case 5: //SPAZIO
                        sky.initSkyBox("textures/spaceft.bmp", "textures/spacert.bmp", "textures/spacelf.bmp", "textures/spacebk.bmp", "textures/spaceup.bmp", "textures/spacedn.bmp");
                        weather_condition = CLEAR;
                        isDay = false;
                        break;
                        
                    case 6: //SERA SERNO
                        sky.initSkyBox("textures/skyft.bmp", "textures/skyrt.bmp", "textures/skylf.bmp", "textures/skybk.bmp", "textures/skyup.bmp", "textures/skydn.bmp");
                        weather_condition = CLEAR;
                        isDay = false;
                        break;
                        
                    case 7: //SERA PIOGGIA
                        sky.initSkyBox("textures/skyft.bmp", "textures/skyrt.bmp", "textures/skylf.bmp", "textures/skybk.bmp", "textures/skyup.bmp", "textures/skydn.bmp");
                        weather_condition = RAIN;
                        isDay = false;
                        break;
                        
                    case 8: //SERA NEVE
                        sky.initSkyBox("textures/skyft.bmp", "textures/skyrt.bmp", "textures/skylf.bmp", "textures/skybk.bmp", "textures/skyup.bmp", "textures/skydn.bmp");
                        weather_condition = SNOW;
                        isDay = false;
                        break;
                        
                    default: //GIORNO SERENO
                        sky.initSkyBox("textures/txStormydays_front.bmp", "textures/txStormydays_right.bmp", "textures/txStormydays_left.bmp", "textures/txStormydays_back.bmp", "textures/txStormydays_up.bmp", "textures/txStormydays_down.bmp");
                        weather_condition = CLEAR;
                }
                
                switch (menu.getCharacter()) {
                    case 0: //BB8
                        character.init("textures/bb8_body.obj","textures/bb8_body.mtl", "textures/bb8_head.obj", "textures/bb8_head.mtl", 0);
                        break;
                    case 1: //D0
                        character.init("textures/d0_body.obj","textures/d0_body.mtl", "textures/d0_head.obj", "textures/d0_head.mtl", 1);
                        break;
                        
                    default:
                        character.init("textures/bb8_body.obj","textures/bb8_body.mtl", "textures/bb8_head.obj", "textures/bb8_head.mtl", 0);
                        break;
                }
            }
            else if(select == 3){
                engine->drop();
                exit(0);
            }
        }
    }
}


void Game::windowSpecial(int key,int x,int y){
    if(isStart){
        if (key == GLUT_KEY_RIGHT){
            if(isAudio)
                engine->play2D("sounds/Jump.wav");
            
            character.setX(1);
        }
        else if (key == GLUT_KEY_LEFT){
            if(isAudio)
                engine->play2D("sounds/Jump.wav");
            
            character.setX(-1);
        }
    }

    redisplayAll();
}

void Game::windowKey(unsigned char key,int x,int y){
    if (key == 27){
        if(isStart){
            if(isPaused){
                obj.initObject();
                character.initialPosition();
                isStart = false;
                isPaused = false;
                speed = 0.1;
                update = 0.1;
            }
        }
        else{
            engine->drop();
            exit(0);
        }
    }
    
    else if(key == 'p' or key == 'P'){
        if(isStart)
            isPaused = !isPaused;
    }
    
    else if(key == 'm' or key == 'M'){
        if(isAudio){
            engine->setSoundVolume(0.0f);
            isAudio = false;
        }
        else{
            engine->setSoundVolume(0.3f);
            isAudio = true;
        }
    }
    
    redisplayAll();
}

void Game::displayProject(double fov, double asp, double dim){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov,asp,dim/16,16*dim);
    glMatrixMode(GL_MODELVIEW);
    glutReshapeWindow( DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT);
}

void Game::displayReshape(int width,int height){
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0, width, height);
    displayProject(fov, asp, dim);
}

void Game::redisplayAll(void){
    displayProject(fov, asp, dim);
    glutPostRedisplay();
}
