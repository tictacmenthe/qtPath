//
// Created by tic-tac on 11/01/18.
//

#ifndef QTSTAR_MAP_H
#define QTSTAR_MAP_H


#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <memory>
#include "../Math/Circle.h"
#include "Node.h"
#include "Edge.h"


class Map : public QFrame {
public:
    Map(QWidget* parent, int p_nbCircles);
    void paintEvent(QPaintEvent* event);

private:

    enum ObstacleState{
        IGNORE,
        NOT_IN_OBSTACLE,
        IN_OBSTACLE
    };

    QTimer timer;


    void clear();

    void addCircle(const Vec &center, int radius);
    void addNode(std::shared_ptr<Node> &position, bool isStart=false, bool isEnd=false);
    void addEdge(std::shared_ptr<Node> &a,std::shared_ptr<Node> &b, bool isStart = false, bool isEnd = false);

    void populateObstacles();
    void populateNodes();
    void populateEdges();

    int checkObstacleInEdge(const Circle &c, const Edge &e, bool verifyDistance=false);

    std::shared_ptr<Node> minDistNode(std::vector<std::shared_ptr<Node>> &tree);


    QPixmap background;
    std::vector<Circle> obstacles;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Edge>> edges;

    int nbCircles;
    int mousex=0;
    int mousey=0;
    bool started;

    std::shared_ptr<Node> startNode;
    std::vector<std::shared_ptr<Edge>> startEdges;
    std::shared_ptr<Node> endNode;
    std::vector<std::shared_ptr<Edge>> endEdges;
    std::shared_ptr<Edge> startEndEdge;
    std::vector<std::shared_ptr<Node>> pathNodes;
    std::vector<std::shared_ptr<Edge>> pathEdges;
    void checkNeighbours(std::shared_ptr<Node> &currentNode);

public slots:

    void mapMovement();
    void populate();
    void start(){
        started=true;
        populate();
    }
    void reset(){
        started=false;
        clear();
    }
    void findPathDijkstra();

private slots:
    void updatePos();
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void removeEdgesOfNode(Node &n);

};


#endif //QTSTAR_MAP_H
