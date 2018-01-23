//
// Created by tic-tac on 19/01/18.
//

#include "Movable.h"

float Movable::getSpeedX() {
    return vx;
}

float Movable::getSpeedY() {
    return vy;
}

void Movable::addSpeeds(float addx, float addy) {
    if(vx<=maxSpeed && vx>=-maxSpeed){
        vx+=addx;
    }
    else if(vx>maxSpeed){
        vx=maxSpeed;
    }
    else if(vx<-maxSpeed){
        vx=-maxSpeed;
    }

    if(vy<=maxSpeed && vy>=-maxSpeed){
        vy+=addy;
    }
    else if(vy>maxSpeed){
        vy=maxSpeed;
    }
    else if(vy<-maxSpeed){
        vy=-maxSpeed;
    }
}
