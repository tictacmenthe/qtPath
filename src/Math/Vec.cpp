//
// Created by tic-tac on 12/01/18.
//

#include "Vec.h"
#include <math.h>


Vec::Vec(int x, int y) : QPoint(x, y) {length=sqrt(x*x+y*y);}
Vec::Vec(const QPoint &point) : QPoint(point) {length=sqrt(point.x()*point.x()+point.y()*point.y());}

/**
 *  Produit scalaire avec un vecteur v2
 * @param v2 vecteur de type Vec
 * @return entier résultant
 */
int Vec::operator*(const Vec &v2) const {
    return (x()*v2.x())+(y()*v2.y());
}

/**
 *  Produit scalaire avec un vecteur qp2
 * @param qp2 vecteur de type QPoint
 * @return entier résultant
 */
int Vec::operator*(const QPoint &qp2) const {
    return (x()*qp2.x())+(y()*qp2.y());
}


bool Vec::operator==(const Vec &n2) const {
    return x()==n2.x() && y()==n2.y();
}


Vec::Vec(float theta, float radius) : QPoint(radius*cos(theta),radius*sin(theta)){
    length=sqrt(x()*x()+y()*y());
}

/**
 *  Distance au point v2
 * @param v2 vecteur
 * @return distance entière
 */
float Vec::dist(const Vec &v2) const {
    return sqrt(pow(x()-v2.x(),2)+pow(y()-v2.y(),2));
}

float Vec::getLength() const {
    return length;
}

Vec::Vec() : QPoint(0,0){}

