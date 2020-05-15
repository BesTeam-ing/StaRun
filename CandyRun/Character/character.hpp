//
//  character.hpp
//  CandyRun
//
//  Created by Ciro De Vita on 13/05/2020.
//  Copyright © 2020 BesTeam-ing. All rights reserved.
//

#ifndef character_hpp
#define character_hpp

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <OpenGL/glext.h>

class Character{
public:
    Character();
    ~Character();
    
    float getX();
    float getY();
    float getZ();
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    
    void setScore(int x);
    void setLife(int x);
    
    int getScore();
    int getLife();
    
    void drawCharacter();
    void initialPosition();
private:
    float x = 0.0f;
    float y = 0.5f;
    float z = 15.0f;
    float size = 0.5f;
    float rotateAngle = 0.0f;
    
    int score;
    int lifes =3;
};

#endif /* character_hpp */
