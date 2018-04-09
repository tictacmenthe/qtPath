//
// Created by tic-tac on 11/01/18.
//

#include "Circle.h"
#include <math.h>
Circle::Circle(Vec pos, int r) :center(pos), radius(r), Movable(5+100.0f/r){}

void Circle::draw(QPainter & painter)const {
    painter.drawEllipse(center,radius,radius);
    painter.drawPoint(center);
}

bool Circle::contains(const Vec &vec) const {
    return (pow(vec.x()-center.x(),2)+pow(vec.y()-center.y(),2))<pow(radius,2);
}

bool Circle::operator==(const Circle &c2) const {
    return center==c2.getCenter();
}
