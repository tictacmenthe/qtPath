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

class Node : public Vec{
public:
    Node():Vec(){}
    Node(const Vec& pos):Vec(pos){}
    Node(int x, int y):Vec(x,y){}


    //Usage:
    void draw(QPainter& painter);
    int distanceTo(const Node &);

    int getCost();
    int getHeuristic();
    void setCost(int);
    void setHeuristic(int);

    bool isVisited(){return visited;}
    void setVisited(){visited=true;}
    void setNotVisited(){visited=false;}

    void setPredecessor(std::shared_ptr<Node>& e);
    std::shared_ptr<Node>& getPredecessor();
    std::vector<std::shared_ptr<Edge>>& getEdges();
    void addEdge(std::shared_ptr<Edge>& e);
    int removeEdge(std::shared_ptr<Edge> &e);
    void clearEdges();

private:
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Node> predecessor=std::shared_ptr<Node>();
    int cost;
    int heuristic;
    bool visited=false;
};


#endif //QTSTAR_NODE_H
