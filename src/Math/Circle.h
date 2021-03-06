//
// Created by tic-tac on 11/01/18.
//

#ifndef QTSTAR_CIRCLE_H
#define QTSTAR_CIRCLE_H


#include <QtCore/QPoint>
#include <QtGui/QPainter>
#include "Vec.h"
#include "../Graph/Movable.h"

class Circle : public Movable{
public:
    Circle(Vec, int);
    //Usage
    void draw(QPainter&) const;
    bool contains(const Vec& vec)const;

    //Getters
    const Vec& getCenter()const{return center;}
    const float getRadius()const{return radius;}
    //Setters
    void setCenter(const Vec& p_pos){center.setX(p_pos.x());center.setY(p_pos.y());}
    void setRadius(float r){radius=r;}

    bool operator==(const Circle& c2)const;
private:
    Vec center;
    int radius;
};


#endif //QTSTAR_CIRCLE_H
