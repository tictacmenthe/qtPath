//
// Created by tic-tac on 4/1/18.
//

#ifndef QTSTAR_GRAPH_H
#define QTSTAR_GRAPH_H


#include "Node.h"
#include "../Math/Circle.h"
#include <random>
#include <QPainter>
#include <QDebug>


enum NodeType{
    DEFAULT,
    START_NODE,
    END_NODE
};

class Graph {

    enum ObstacleState{
        IGNORE,
        NOT_IN_OBSTACLE,
        IN_OBSTACLE
    };

    static constexpr int DijkstraMaxDistance=100000000;

    int nbCircles;
    int width;
    int height;
    QRect parent;

    std::vector<Circle> obstacles;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Edge>> edges;

    std::shared_ptr<Node> startNode;
    std::vector<std::shared_ptr<Edge>> startEdges;
    std::shared_ptr<Node> endNode;
    std::vector<std::shared_ptr<Edge>> endEdges;
    std::shared_ptr<Edge> startEndEdge;
    std::vector<std::shared_ptr<Node>> pathNodes;
    std::vector<std::shared_ptr<Edge>> pathEdges;

    ObstacleState checkObstacleInEdge(const Circle &c, const Edge &e);

    void populateObstacles();
    void addCircle(const Vec &center, int radius);
    void populateNodes();
    void addNode(std::shared_ptr<Node> &position);
    bool checkNodeCreation(std::shared_ptr<Node> &position);
    void populateEdges();
    void addEdge(std::shared_ptr<Node> &a, std::shared_ptr<Node> &b, NodeType type=DEFAULT);
    void removeEdgesOfNode(Node &n);
    void removeEdgeStartEndNodes();


    std::shared_ptr<Node> minDistNode(std::vector<std::shared_ptr<Node>> &tree);
    void checkNeighbours(std::shared_ptr<Node> &currentNode);
    void findPathDijkstra();
    void findPathAstar();

public:

    Graph(int p_nbCircles, int width, int height, QRect parentRect);

    void populate();
    void addPathNode(std::shared_ptr<Node> &position, NodeType type);
    void findPath();

    void paint(QPainter& painter);
    void update();
    void clear();
    void resize(float hratio, float wratio, const QRect newRect);


};


#endif //QTSTAR_GRAPH_H
