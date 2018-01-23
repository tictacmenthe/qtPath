//
// Created by tic-tac on 12/01/18.
//

#include "Node.h"

void Node::draw(QPainter &painter) {
    painter.drawPoint(*this);
}

int Node::dist(const Node &n) {
    int value=0;
    value=sqrt(pow(x()-n.x(),2)+pow(y()-n.y(),2));
    return value;
}

void Node::addEdge(std::shared_ptr<Edge> &e) {
    edges.push_back(e);
}

int Node::removeEdge(std::shared_ptr<Edge>& e){
    auto it=std::find(edges.begin(),edges.end(),e);
    if(it!=edges.end()) {
        edges.erase(it);
    }
 }

std::vector<std::shared_ptr<Edge>> &Node::getEdges() {
    return edges;
}

int Node::getDistance() {
    return distance;
}

void Node::setDistance(int d) {
    distance=d;
}

void Node::setPredecessor(std::shared_ptr<Node> &e) {
    predecessor=e;
}

std::shared_ptr<Node>& Node::getPredecessor() {
    return predecessor;
}
