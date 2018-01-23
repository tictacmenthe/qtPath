//
// Created by tic-tac on 12/01/18.
//

#ifndef QTSTAR_VEC_H
#define QTSTAR_VEC_H


#include <QtCore/QPoint>

class Vec : public QPoint {
public:
    Vec(int x, int y);
    Vec(float theta, float radius);
    Vec(const QPoint& point);
    Vec();

    int operator*(const Vec& v2) const;
    int operator*(const QPoint& qp2) const;
    bool operator==(const Vec& vc2)const;

    float dist(const Vec& v2)const;
    float getLength()const;

private:
    float length;
};


#endif //QTSTAR_VEC_H
