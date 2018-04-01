//
// Created by tic-tac on 11/01/18.
//


#include "MapFrame.h"


#define TIMER_FREQ   24
#define TIMER_PERIOD (int)1000/TIMER_FREQ

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define CHECKTIME(x)  \
    QElapsedTimer CONCAT(sb_, __LINE__); \
    CONCAT(sb_, __LINE__).start(); \
    x \
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " Elapsed time: " <<  CONCAT(sb_, __LINE__).nsecsElapsed() << " ns.";

MapFrame::MapFrame(QWidget *parent, int p_nbCircles) : graph(p_nbCircles, width(),height(), rect()){
    //Loading the background image if needed
    background.load("../map.png");

    setMouseTracking(true);

    //Enables event filtering
    installEventFilter(this);

    //Starts update timer for animations
    //connect(&timer, &QTimer::timeout,this,&MapFrame::updatePos);

    //Used to check whether a map was generated or not(for resize)
    started=false;
}

void MapFrame::mouseReleaseEvent(QMouseEvent *event) {
//    if (event->button()==Qt::MouseButton::LeftButton) {
//        std::shared_ptr<Node> node=std::make_shared<Node>(Node(mousex,mousey));
//        auto it=std::find_if(edges.begin(),edges.end(),[&](const std::shared_ptr<Edge>& e){return (e->getB()==*startNode && e->getA()==*endNode)||(e->getA()==*startNode && e->getB()==*endNode);});
//        if(it!=edges.end()){
//            edges.erase(it);
//        }
//        addNode(node, true, false);
//        if(*endNode!=Node())
//            addEdge(startNode,endNode);
//        findPathDijkstra();
//    } else if(event->button()==Qt::MouseButton::RightButton) {
//        std::shared_ptr<Node> node=std::make_shared<Node>(Node(mousex,mousey));
//        auto it=std::find_if(edges.begin(),edges.end(),[&](const std::shared_ptr<Edge>& e){return (e->getB()==*startNode && e->getA()==*endNode)||(e->getA()==*startNode && e->getB()==*endNode);});
//        if(it!=edges.end()){
//            edges.erase(it);
//        }
//        addNode(node, false, true);
//        if(*startNode!=Node())
//            addEdge(startNode,endNode);
//        findPathDijkstra();
//    }
    update();
}

void MapFrame::mouseMoveEvent(QMouseEvent *event) {
    mousex=event->x();
    mousey=event->y();

    update();
}

void MapFrame::resizeEvent(QResizeEvent *event) {
    //TODO resize proportionnal with event->size() and event->oldsize()
}

void MapFrame::mapMovement(){
    if(timer.isActive()) {
        timer.stop();
    }
    else{
        timer.start(TIMER_PERIOD);
    }
}

void MapFrame::updatePos(){
    graph.update();
    update();
}


void MapFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap));
    //Background Color
    //painter.fillRect(rect(),Qt::darkGray);
    painter.drawRect(rect());
    painter.fillRect(rect(),QColor(0x59,0x59,0x59));
    //painter.drawPixmap(0,0,width(), height(), background);

    graph.paint(painter);

    painter.drawText(20,20,QString("x: ").append(QString::number(mousex)));
    painter.drawText(20,40,QString("y: ").append(QString::number(mousey)));
}


/*********************************************************/
/*                      PATHFINDING                      */
/*********************************************************/



void MapFrame::findPathDijkstra(){
    qDebug()<<"===================================================================";

    update();
}