//
// Created by tic-tac on 11/01/18.
//

#ifndef QTSTAR_MAP_H
#define QTSTAR_MAP_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <memory>
#include <QDebug>
#include <QtCore/QElapsedTimer>

#include "../Graph/Graph.h"



class MapFrame : public QFrame {
    Q_OBJECT

    Graph graph;
    QTimer timer;
    QPixmap background;

    void findPathDijkstra();

    int mousex=0;
    int mousey=0;

    bool started;
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void mapMovement();
    void updatePos();

    void start(){
        started=true;
        graph.populate();
        update();
    }
    void reset(){
        started=false;
        graph.clear();
        update();
    }

public:
    MapFrame(QWidget* parent, int p_nbCircles);
    void paintEvent(QPaintEvent* event);

};


#endif //QTSTAR_MAP_H
