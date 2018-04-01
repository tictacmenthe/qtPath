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


class Graph {

    enum ObstacleState{
        IGNORE,
        NOT_IN_OBSTACLE,
        IN_OBSTACLE
    };


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

    int checkObstacleInEdge(const Circle &c, const Edge &e, bool verifyDistance=false);

    void addCircle(const Vec &center, int radius);
    void addNode(std::shared_ptr<Node> &position, bool isStart=false, bool isEnd=false);
    void addEdge(std::shared_ptr<Node> &a,std::shared_ptr<Node> &b, bool isStart = false, bool isEnd = false);
    void removeEdgesOfNode(Node &n);

    void populateObstacles();
    void populateNodes();
    void populateEdges();

    std::shared_ptr<Node> minDistNode(std::vector<std::shared_ptr<Node>> &tree);
    void checkNeighbours(std::shared_ptr<Node> &currentNode);
    void findPathDijkstra();

public:
    Graph(int p_nbCircles, int width, int height, QRect parentRect);
    void paint(QPainter& painter);

    void populate();
    void clear();

    void update();
};


#endif //QTSTAR_GRAPH_H
