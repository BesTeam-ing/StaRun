//
//  menu.cpp
//  StaRun
//
//  Created by Ciro De Vita and Gennaro Mellone on 13/05/2020.
//  Copyright © 2020 BesTeam-ing. All rights reserved.
//

#include "menu.hpp"

Server server;
Character _character;
objloader _title;

//struct polygon
typedef struct polygon{
     float x1,y1,x2,y2;
} polygon;
  
polygon polygons[15];
int scores[2];

GLfloat lp_title[] = {0.5, 0.5, 0.9, 0.4}; //light position

int temp = 0, temp_night = 6; //temp variables

//BACKGROUND
int i = 0;
const int BACKGROUND_COUNT = 6;
const char* BACKGROUND[] = {
    "textures/sky191up.bmp",
    "textures/sky303up.bmp",
    "textures/sky303up.bmp",
    "",
    "textures/pinkup.bmp",
    "textures/spaceup.bmp"
};

//CHARACTERS
int j = 0;
const int CHARACTER_COUNT = 2;
const char* CHARACTER[] = {
    "textures/bb8_icon.png",
    "textures/d0_icon.png"
};

//WEATHER
int k = 0;
const int WEATHER_COUNT = 6;
const char* WEATHER[] = {
    "textures/sun.png",
    "textures/rainy.png",
    "textures/snow.png",
    "textures/meteo_logo.png",
    "textures/mars.png",
    "textures/space.png"
};

//textures
unsigned int texture,
    texture_back[BACKGROUND_COUNT],
    texture_character[CHARACTER_COUNT],
    texture_weather[WEATHER_COUNT],
    arrow_left,
    arrow_right,
    background_left,
    background_right,
    title;

bool isFirst = true; // For server call
bool isHighscore = false; //Open HighScore menu

char _scoreBB8[20];
char _scoreD0[20];

const char* connection = ""; //Connection string

Menu::Menu(){}
Menu::~Menu(){}

//function that return backgorund choosen
int Menu::getBackground(){
    //get current time
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int hour=aTime->tm_hour;

    //check day/night
    if(hour >= 19 or hour <= 5){ //If it is night use switch to change weather
        switch (this->background) {
            case 0:
                return 6;
                break;
            case 1:
                return 7;
                break;
            case 2:
                return 8;
                break;
            case 4:
                return 4;
                break;
            case 5:
                return 5;
                break;
            
            default:
                return temp_night;
                break;
        }
    }
    else{
        if(this->background == 3){
            return temp;
        }
        else
            return this->background;
    }
}

//function that return character choosen
int Menu::getCharacter(){
    return this->character;
}

//menu initialization
void Menu::initMenu(){
    //MENU BUTTONS z=-1
    
    //START
    polygons[0].x1=-4.0;
    polygons[0].x2=-1.0;
    polygons[0].y1=1.0;
    polygons[0].y2=1.75;
    //HIGH SCORE
    polygons[1].x1=-4.0;
    polygons[1].x2=-1.0;
    polygons[1].y1=-0.25;
    polygons[1].y2=0.5;
    //EXIT
    polygons[2].x1=-4.0;
    polygons[2].x2=-1.0;
    polygons[2].y1=-1.50;
    polygons[2].y2=-0.75;
    
    //TITLE z = -1
    polygons[3].x1=0.0;
    polygons[3].x2=4.0;
    polygons[3].y1=-1.0;
    polygons[3].y2=-3.5;
    
    //NAME
    polygons[4].x1=0.0;
    polygons[4].x2=4.0;
    polygons[4].y1=1.75;
    polygons[4].y2=-0.75;
    
    polygons[5].x1=-3.0;
    polygons[5].x2=3.0;
    polygons[5].y1=2.5;
    polygons[5].y2=3.25;
    
    //BACKGROUND z = 0
    polygons[6].x1=-4.0;
    polygons[6].x2=4.0;
    polygons[6].y1=4.0;
    polygons[6].y2=-4.0;
    
    //z=-2
    //MENU CHARACTER
    //CHARACTER ICONS
    polygons[7].x1=1.50;
    polygons[7].x2=2.5;
    polygons[7].y1=1.0;
    polygons[7].y2=-0.5;
    
    //ARROW LEFT
    polygons[9].x1=0.25;
    polygons[9].x2=1.0;
    polygons[9].y1=1.0;
    polygons[9].y2=-0.25;
    
    //ARROW RIGHT
    polygons[10].x1=3.25;
    polygons[10].x2=4.0;
    polygons[10].y1=1.0;
    polygons[10].y2=-0.25;
    
    //MENU WEATHER
    //WEATHER ICONS
    polygons[8].x1=1.5;
    polygons[8].x2=2.5;
    polygons[8].y1=-1.75;
    polygons[8].y2=-3.25;

    //ARROW LEFT 1
    polygons[11].x1=0.25;
    polygons[11].x2=1.0;
    polygons[11].y1=-1.75;
    polygons[11].y2=-3.0;
    
    //ARROW RIGHT 1
    polygons[12].x1=3.25;
    polygons[12].x2=4.0;
    polygons[12].y1=-1.75;
    polygons[12].y2=-3.0;
    
    //HIGHSCORE
    polygons[13].x1=-2.0;
    polygons[13].x2=2.0;
    polygons[13].y1=2.0;
    polygons[13].y2=-2.0;
    
    //METEO
    polygons[14].x1=-3.8;
    polygons[14].x2=-2.3;
    polygons[14].y1=-2.5;
    polygons[14].y2=-3.5;
    
    //TITLE
    title = _title.load("textures/Starun.obj","textures/Starun.mtl");
    
    //TEXTURE MENU ITEMS
    texture = SOIL_load_OGL_texture("textures/logo.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    background_left = SOIL_load_OGL_texture("textures/background_button2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    background_right = SOIL_load_OGL_texture("textures/background_button1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    arrow_left = SOIL_load_OGL_texture("textures/left-arrow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    arrow_right = SOIL_load_OGL_texture("textures/right-arrow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
    //TEXTURE WEATHER BACKGROUND
    for(int j=0; j<BACKGROUND_COUNT; j++){
        texture_back[j] = SOIL_load_OGL_texture(BACKGROUND[j], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    }
    
    //TEXTURE CHARACTERS
    for (int j=0; j<CHARACTER_COUNT; j++){
        texture_character[j] = SOIL_load_OGL_texture(CHARACTER[j], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    }
    
    //WEATHER ICONS
    for (int j=0; j<WEATHER_COUNT; j++){
        texture_weather[j] = SOIL_load_OGL_texture(WEATHER[j], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    }
    
}

//function to draw menu
void Menu::draw(){
    //HIGHSCORES
    if(isHighscore){
        glPushMatrix();
        glColor4f(0.0, 0.0, 0.0,0.4);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f,0.0f);
                glVertex3f(polygons[13].x1, polygons[13].y1, -3);
                glTexCoord2f(0.0f,1);
                glVertex3f(polygons[13].x1, polygons[13].y2, -3);
                glTexCoord2f(1.0f,1);
                glVertex3f(polygons[13].x2, polygons[13].y2, -3);
                glTexCoord2f(1.0f,0.0f);
                glVertex3f(polygons[13].x2, polygons[13].y1, -3);
            glEnd();
        glPopMatrix();
        
        //display highscore
        _character.ReadHighScore();
        sprintf(_scoreBB8, "%d", _character.getBB8score());
        sprintf(_scoreD0, "%d", _character.getD0score());
        this->DrawText(-1, 1.5, -4, "Highscore" ,25.0);
        this->DrawText(-1.5, 0, -4, "BB8:" ,25.0);
        this->DrawText(0, 0, -4, _scoreBB8 ,15.0);
        this->DrawText(-1.5, -1, -4, "D0:" ,25.0);
        this->DrawText(0, -1, -4, _scoreD0 ,15.0);
    }

    //thread to update weather condition asynchronously
    //check if it's the first time
    if(isFirst){
        isFirst = false;
        connection = "In Connection ...";
        texture_back[3] = texture_back[5];
        std::thread([](){
            std::string api = server.getWeather(); //get weather condition
            connection = "Connected!";
            std::transform(api.begin(), api.end(), api.begin(), ::tolower); //string tolower
            
            //string comparison
            if(api.find("sereno") != std::string::npos){
                texture_back[3] = texture_back[0];
                temp = 0;
                temp_night = 6;
            }
            else if(api.find("nuvol") != std::string::npos or api.find("pioggia") != std::string::npos or api.find("neve") != std::string::npos or api.find("rovesci") != std::string::npos){
                texture_back[3] = texture_back[1];
                temp = 1;
                temp_night = 7;
            }
            else{
                texture_back[3] = texture_back[2];
                temp = 2;
                temp_night = 8;
            }
            
        }).detach(); //thread end
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_back[this->background]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //background Z=0
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[6].x1, polygons[6].y1, 2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[6].x1, polygons[6].y2, 2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[6].x2, polygons[6].y2, 2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[6].x2, polygons[6].y1, 2);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    //title
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glScalef(0.6, 0.6, 0.6);
        glTranslatef(0, 0, 0);
        glRotatef(15, 0, 0, 1);
        glLightfv(GL_LIGHT0,GL_POSITION,lp_title);
        glCallList(title);
    glPopMatrix();
    
    
    //Z = -1
    //METEO ICON
    DrawText(-3.8,-3.8,-2,connection,15.0);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3f(1.0, 1.0, 1.0);
               glBegin(GL_QUADS);
                   glTexCoord2f(0.0f,0.0f);
                   glVertex3f(polygons[14].x1, polygons[14].y1, -1);
                   glTexCoord2f(0.0f,1);
                   glVertex3f(polygons[14].x1, polygons[14].y2, -1);
                   glTexCoord2f(1.0f,1);
                   glVertex3f(polygons[14].x2, polygons[14].y2, -1);
                   glTexCoord2f(1.0f,0.0f);
                   glVertex3f(polygons[14].x2, polygons[14].y1, -1);
               glEnd();
            glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    DrawText(-3.8,-2.5,-2,"Powered By:",15.0);
    
    //button background
    for(int i=0;i<6;i++){
        glEnable(GL_TEXTURE_2D);
        if (i == 0 or i == 1 or i == 2)
            glBindTexture(GL_TEXTURE_2D, background_left);
        else
            glBindTexture(GL_TEXTURE_2D, background_right);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0,0.7);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(polygons[i].x1, polygons[i].y1, -1);
            glTexCoord2f(0.0f,1);
            glVertex3f(polygons[i].x1, polygons[i].y2, -1);
            glTexCoord2f(1.0f,1);
            glVertex3f(polygons[i].x2, polygons[i].y2, -1);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(polygons[i].x2, polygons[i].y1, -1);
        glEnd();
        glDisable(GL_BLEND);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    
    //Z=-2
    //CHARACTER ICONS
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_character[this->character]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[7].x1, polygons[7].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[7].x1, polygons[7].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[7].x2, polygons[7].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[7].x2, polygons[7].y1, -2);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //WEATHER ICONS
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_weather[this->weather]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[8].x1, polygons[8].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[8].x1, polygons[8].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[8].x2, polygons[8].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[8].x2, polygons[8].y1, -2);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //ARROWS LEFT
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, arrow_left);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[9].x1, polygons[9].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[9].x1, polygons[9].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[9].x2, polygons[9].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[9].x2, polygons[9].y1, -2);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[11].x1, polygons[11].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[11].x1, polygons[11].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[11].x2, polygons[11].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[11].x2, polygons[11].y1, -2);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //ARROWS RIGHT
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, arrow_right);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[10].x1, polygons[10].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[10].x1, polygons[10].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[10].x2, polygons[10].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[10].x2, polygons[10].y1, -2);
    glEnd();
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(polygons[12].x1, polygons[12].y1, -2);
        glTexCoord2f(0.0f,1);
        glVertex3f(polygons[12].x1, polygons[12].y2, -2);
        glTexCoord2f(1.0f,1);
        glVertex3f(polygons[12].x2, polygons[12].y2, -2);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(polygons[12].x2, polygons[12].y1, -2);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    DrawText(-2.0,2.75,-2,"StaRun - C. De Vita G. Mellone",20.0);
    DrawText(-2.9,1.25,-2,"Start",25.0);
    DrawText(-3.3,0.0,-2,"High Score",25.0);
    DrawText(-2.9,-1.25,-2,"Exit",25.0);
    DrawText(1.5, 1.20,-2,"Character",25.0);
    DrawText(1.5,-1.50,-2,"Weather",25.0);
}

// Used to draw menu and clean buffer
void Menu::drawMenu(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    
    glScalef(0.25,0.25,0.25);
    
    this->draw();
    
    glFlush();
    glutSwapBuffers();
}

//function to draw some text
void Menu::DrawText(float x,float y, float z, const char *text, int size){
    float _size = 0.0001*size;
        glPushMatrix();
        glColor4f(1.0, 1.0, 0.0, 0.0);
        glTranslatef(x, y, z);
        glScalef(_size, _size, _size);
        for( const char* p = text; *p; p++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
        }
    glPopMatrix();
}

//function to handle mouse interaction
int Menu::select(GLint x, GLint y){
    
    if(isHighscore){
        isHighscore = false;
    }
    else{
        if((x>=0 && x<=460) && (y>=210 && y<=270))
            return 1;
        
        else if((x>=0 && x<=460) && (y>=320 && y<=380)){
            isHighscore = true;
        }
        
        else if((x>=0 && x<=460) && (y>=430 && y<=490))
             return 3;
        
        else if((x>=665 && x<=775) && (y>=270 && y<=400)){
            j--;
            this->character = (j % CHARACTER_COUNT + CHARACTER_COUNT) % CHARACTER_COUNT;
        }
        
        else if((x>=1145 && x<=1250) && (y>=270 && y<=400)){
            j++;
            this->character = (j % CHARACTER_COUNT + CHARACTER_COUNT) % CHARACTER_COUNT;
        }
        
        else if((x>=665 && x<=775) && (y>=515 && y<=650)){
            i--;
            this->background = (i % BACKGROUND_COUNT + BACKGROUND_COUNT) % BACKGROUND_COUNT;
            k--;
            this->weather = (k % WEATHER_COUNT + WEATHER_COUNT) % WEATHER_COUNT;
        }
        
        else if((x>=1145 && x<=1250) && (y>=515 && y<=650)){
            i++;
            this->background = (i % BACKGROUND_COUNT + BACKGROUND_COUNT) % BACKGROUND_COUNT;
            k++;
            this->weather = (k % WEATHER_COUNT + WEATHER_COUNT) % WEATHER_COUNT;
        }
    }
    
    return -1;
}

