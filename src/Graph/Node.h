//
// Created by tic-tac on 12/01/18.
//

#ifndef QTSTAR_NODE_H
#define QTSTAR_NODE_H


#include <QtGui/QPainter>
#include "../Math/Vec.h"
#include "Edge.h"
#include <cmath>
#include <memory>
#include "Movable.h"

class Edge;

class Node : public Vec, public Movable{
public:
    Node():Vec(),Movable(maxSpeed){}
    Node(const Vec& pos):Vec(pos),Movable(maxSpeed){}
    Node(int x, int y):Vec(x,y),Movable(maxSpeed){}


    //Usage:
    void draw(QPainter& painter);
    int dist(const Node&);

    int getDistance();
    void setDistance(int);

    bool isVisited(){return visited;}
    bool setVisited(){visited=true;}
    bool setNotVisited(){visited=false;}

    void setPredecessor(std::shared_ptr<Node>& e);
    std::shared_ptr<Node>& getPredecessor();
    std::vector<std::shared_ptr<Edge>>& getEdges();
    void addEdge(std::shared_ptr<Edge>& e);
    int removeEdge(std::shared_ptr<Edge> &e);

private:

    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Node> predecessor=std::shared_ptr<Node>();
    int distance=3000;
    bool visited=false;
    static constexpr int maxSpeed=2;
};


#endif //QTSTAR_NODE_H
