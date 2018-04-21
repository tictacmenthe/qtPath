//
// Created by tic-tac on 17/01/18.
//

#include "Edge.h"

Edge::Edge(std::shared_ptr<Node> a, std::shared_ptr<Node> b) : nodeA(a),nodeB(b){
    center=(*a+*b)/2;
    length= b->distanceTo(*a);
    direction=(*b)-(*a);
}

void Edge::draw(QPainter &painter) {
    painter.drawLine(*nodeA,*nodeB);
}

Node& Edge::getA() const{
    return *nodeA;
}
Node& Edge::getB() const{
    return *nodeB;
}

bool Edge::operator==(const Edge &e)const {
    return e.getA()==*nodeA && e.getB()==*nodeB;
}

const Vec &Edge::getCenter() const {
    return center;
}

int Edge::getLength() const {
    return length;
}

float Edge::angleTo(const Vec& v)const{
    return atan2(v.y(),v.x())-atan2(direction.y(),direction.x());
}
