//
// Created by tic-tac on 19/01/18.
//

#ifndef QTSTAR_MOVABLE_H
#define QTSTAR_MOVABLE_H


class Movable {
public:

    Movable(int p_maxSpeed):maxSpeed(p_maxSpeed){}
    float getSpeedX();
    float getSpeedY();
    void addSpeeds(float,float);
protected:
    float vx=0;
    float vy=0;
    int maxSpeed;
};


#endif //QTSTAR_MOVABLE_H
