//
// Created by tic-tac on 17/01/18.
//

#ifndef QTSTAR_EDGE_H
#define QTSTAR_EDGE_H

#include "Node.h"
#include <memory>
class Node;

class Edge {
public:
    Edge(std::shared_ptr<Node> a,std::shared_ptr<Node> b);

    void draw(QPainter& painter);

    Node& getA()const;
    Node& getB()const;
    const Vec& getCenter()const;
    int getLength()const;

    float angleTo(const Vec &v)const;


    bool operator==(const Edge& e)const;


private:
    std::shared_ptr<Node> nodeA;
    std::shared_ptr<Node> nodeB;

    Vec center;
    Vec direction;
    int length;

};


#endif //QTSTAR_EDGE_H
