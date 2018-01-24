//
// Created by tic-tac on 11/01/18.
//

#include <iostream>
#include "Map.h"
#include "../Math/Math.h"
#include <QDebug>
#include <random>
#include <QtCore/QTime>
#include <QtCore/QElapsedTimer>
#include <deque>

#define TIMER_FREQ   24
#define TIMER_PERIOD (int)1000/TIMER_FREQ

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define CHECKTIME(x)  \
    QElapsedTimer CONCAT(sb_, __LINE__); \
    CONCAT(sb_, __LINE__).start(); \
    x \
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " Elapsed time: " <<  CONCAT(sb_, __LINE__).nsecsElapsed() << " ns.";

Map::Map(QWidget *parent, int p_nbCircles) {
    background.load("../map.png");
    nbCircles=p_nbCircles;

    setMouseTracking(true);

    installEventFilter(this);

    startNode=std::make_shared<Node>();
    endNode=std::make_shared<Node>();

    connect(&timer, &QTimer::timeout,this,&Map::updatePos);
    started=false;
}

void Map::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button()==Qt::MouseButton::LeftButton) {
        std::shared_ptr<Node> node=std::make_shared<Node>(Node(mousex,mousey));
        auto it=std::find_if(edges.begin(),edges.end(),[&](const std::shared_ptr<Edge>& e){return (e->getB()==*startNode && e->getA()==*endNode)||(e->getA()==*startNode && e->getB()==*endNode);});
        if(it!=edges.end()){
            edges.erase(it);
        }
        addNode(node, true, false);
        if(*endNode!=Node())
            addEdge(startNode,endNode);
        findPathDijkstra();
    } else if(event->button()==Qt::MouseButton::RightButton) {
        std::shared_ptr<Node> node=std::make_shared<Node>(Node(mousex,mousey));
        auto it=std::find_if(edges.begin(),edges.end(),[&](const std::shared_ptr<Edge>& e){return (e->getB()==*startNode && e->getA()==*endNode)||(e->getA()==*startNode && e->getB()==*endNode);});
        if(it!=edges.end()){
            edges.erase(it);
        }
        addNode(node, false, true);
        if(*startNode!=Node())
            addEdge(startNode,endNode);
        findPathDijkstra();
    }
    update();
}

void Map::mouseMoveEvent(QMouseEvent *event) {
    mousex=event->x();
    mousey=event->y();

    update();
}

void Map::resizeEvent(QResizeEvent *event) {
    if(started) {
        populate();
    }
}

void Map::mapMovement(){
    if(timer.isActive()) {
        timer.stop();
    }
    else{
        timer.start(TIMER_PERIOD);
    }
}

void Map::populate(){
    clear();

    nodes.push_back(std::shared_ptr<Node>(new Node(1,1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(1,height()-1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(width()-1,1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(width()-1,height()-1)));

    static int timerCount;
    QElapsedTimer timer;

    timer.start();
    populateObstacles();
    populateNodes();
    populateEdges();
    timerCount+=timer.nsecsElapsed()/1000;

    timer.restart();
    update();
}

void Map::populateObstacles(){
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<float> sizeDist(10,height()/8+20);
    for(int i=0;i<nbCircles;){
        int size=Math::abs(sizeDist(generator))+10;
        std::uniform_int_distribution<int> xDist(size,width()-size);
        std::uniform_int_distribution<int> yDist(size,height()-size);
        int cx=xDist(generator);
        int cy=yDist(generator);
        bool add=true;
        for(const Circle& c:obstacles){
            if(c.contains({cx,cy})){
                add=false;
                break;
            }
            else if(c.getCenter().dist({cx,cy})<=(c.getRadius()) || c.getCenter().dist({cx,cy})<=(size)){
                add=false;
                break;
            }
        }
        if(add) {
            addCircle({cx,cy},size);
            i++;
        }
    }
}

void Map::populateNodes() {
    for(const Circle& c:obstacles){
        int nbNodes=sqrt(c.getRadius());
        for(int i=0;i<nbNodes;i++){
            std::shared_ptr<Node> position=std::make_shared<Node>(c.getCenter()+Vec((float)(i*2*3.14/nbNodes), (float)(c.getRadius()*1.05+10)));
            addNode(position);
        }
    }
}

void Map::populateEdges(){
    int nbNodes=nodes.size();
    for(int i=0;i<nbNodes-1;i++) {
        for (int j = i + 1; j < nbNodes; j++) {
            addEdge(nodes.at(i), nodes.at(j));
        }
    }
}

void Map::addCircle(const Vec &center, int radius) {
    obstacles.emplace_back(center, radius);
}

void Map::addNode(std::shared_ptr<Node>& position, bool isStart, bool isEnd){
    bool add=rect().contains(*position);
    for(const Circle& c:obstacles){
        if(c.contains(*position)){
            add=false;
            break;
        }
    }
    for(const auto& n:nodes){
        Circle minDistCircle(*n,5);
        if(minDistCircle.contains(*position)){
            add=false;
            break;
        }
    }
    if(add){
        if(isStart) {
            removeEdgesOfNode(*startNode);
            startNode=position;
            startEdges.clear();
            for(auto& n:nodes){
                addEdge(startNode, n, true,false);
            }
        }
        else if(isEnd){
            removeEdgesOfNode(*endNode);
            endEdges.clear();
            endNode=position;
            for(auto& n:nodes){
                addEdge(endNode, n, false,true);
            }
        }
        else{
            nodes.push_back(position);
        }
    }
}

void Map::addEdge(std::shared_ptr<Node> &a,std::shared_ptr<Node> &b, bool isStart, bool isEnd) {
    int add=NOT_IN_OBSTACLE;
    Edge e(a,b);
    for(const Circle& c:obstacles){
        add=checkObstacleInEdge(c,e);
        if(add==IGNORE){
            add=NOT_IN_OBSTACLE;
            continue;
        }
        else if(add==IN_OBSTACLE){
            break;
        }
    }
    if(add==NOT_IN_OBSTACLE){
        if(isStart){
            startEdges.emplace_back(std::make_shared<Edge>(a,b));
            a->addEdge(startEdges.back());
            b->addEdge(startEdges.back());
        }
        else if(isEnd){
            endEdges.emplace_back(std::make_shared<Edge>(a,b));
            a->addEdge(endEdges.back());
            b->addEdge(endEdges.back());
        }
        else{
            edges.emplace_back(std::make_shared<Edge>(a,b));
            a->addEdge(edges.back());
            b->addEdge(edges.back());
        }
    }
}

void Map::clear(){
    obstacles.clear();
    nodes.clear();
    edges.clear();
    startNode=std::make_shared<Node>();
    startEdges.clear();
    endNode=std::make_shared<Node>();
    endEdges.clear();
    pathNodes.clear();
    pathEdges.clear();
    update();
}

void Map::updatePos(){
    std::default_random_engine generator(time(nullptr));
    std::normal_distribution<float> distribution(0,0.2);

    for(Circle& c: obstacles){
        float addvx=distribution(generator);
        float addvy=distribution(generator);
        c.addSpeeds(addvx,addvy);
        c.setCenter(c.getCenter()+Vec(c.getSpeedX(),c.getSpeedY()));
        if(!rect().contains(c.getCenter())){
            obstacles.erase(std::find(obstacles.begin(),obstacles.end(),c));
        }
    }

    for(auto itn=nodes.begin(); itn!=nodes.end();){
        auto& n=**itn;
        bool toErase=false;
        float addvx=distribution(generator);
        float addvy=distribution(generator);
        n.addSpeeds(addvx,addvy);

        n.setX(n.x()+n.getSpeedX());
        n.setY(n.y()+n.getSpeedY());

        for(auto& c:obstacles){
            if(c.contains(n)){
                toErase=true;
                break;
            }
        }
        if(!rect().contains(n) || toErase){
            removeEdgesOfNode(n);
            itn=nodes.erase(itn);
        }
        else{
            itn++;
        }
    }

    //Deletion of edges that intersect with circles
    for(auto ite=edges.begin();ite!=edges.end();){
        auto& e=**ite;
        int state=NOT_IN_OBSTACLE;
        for(const auto& c:obstacles){
            state=checkObstacleInEdge(c,e,true);
            if(state==IGNORE){
                state=NOT_IN_OBSTACLE;
                continue;
            }
            else if(state==IN_OBSTACLE){
                break;
            }
        }
        if(state==IN_OBSTACLE){
            e.getA().removeEdge(*ite);
            e.getB().removeEdge(*ite);
            ite=edges.erase(ite);
        }
        else{
            ite++;
        }
    }
    update();
}

int Map::checkObstacleInEdge(const Circle &c, const Edge &e, bool verifyDistance) {
    if(c.getCenter().dist(e.getCenter())-c.getRadius() > e.getLength()/2){
        return IGNORE;
    }
    Vec f =c.getCenter()-(e.getA());
    float angle=e.angleTo(f);
    int distance=abs(f.getLength()*sin(angle));
    if(distance <= c.getRadius()){
        return IN_OBSTACLE;
    }
    return NOT_IN_OBSTACLE;
}

void Map::removeEdgesOfNode(Node &n){
    for(auto ite=n.getEdges().begin();ite!=n.getEdges().end();ite++){
        auto& it=*ite;
        auto toRemove=std::find(edges.begin(),edges.end(),it);
        if(toRemove!=edges.end()){
            edges.erase(toRemove);
        }
        toRemove=std::find(startEdges.begin(),startEdges.end(),it);
        if(toRemove!=startEdges.end()){
            startEdges.erase(toRemove);
        }
        toRemove=std::find(endEdges.begin(),endEdges.end(),it);
        if(toRemove!=endEdges.end()){
            endEdges.erase(toRemove);
        }
    }
}

void Map::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap));
    //Background Color
    //painter.fillRect(rect(),Qt::darkGray);
    painter.drawRect(rect());
    //painter.drawPixmap(0,0,width(), height(), background);

    //Lines on the table
    painter.setBrush(Qt::black);
    //painter.drawEllipse(circleX,circleY,100,100);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(0.4);
    for(Circle& c:obstacles){
        c.draw(painter);
    }
    painter.setOpacity(1);
    painter.setPen(QPen(Qt::red,3));
    for(auto& n:nodes){
        n->draw(painter);
    }
    painter.setPen(QPen(Qt::green,5));

    startNode->draw(painter);
    painter.setPen(QPen(Qt::red,5));

    endNode->draw(painter);

    painter.setPen(QPen(Qt::yellow,0.08));
    for(auto& e:edges){
        e->draw(painter);
    }
    painter.setPen(QPen(Qt::green,0.2));
    for(auto& e:startEdges){
        e->draw(painter);
    }
    painter.setPen(QPen(Qt::red,0.2));
    for(auto& e:endEdges){
        e->draw(painter);
    }
    painter.setPen(QPen(Qt::white,4));
    for(auto& n:pathNodes){
        n->draw(painter);
    }
    for(auto& e:pathEdges){
        e->draw(painter);
    }
    painter.setFont(QFont("Arial",10));
    painter.setPen(QPen(Qt::red,1));
    painter.drawText(20,20,QString("x: ").append(QString::number(mousex)));
    painter.drawText(20,40,QString("y: ").append(QString::number(mousey)));
    painter.drawText(width()-100,height()-50,QString("C: ").append(QString::number(obstacles.size())));
    painter.drawText(width()-100,height()-30,QString("N: ").append(QString::number(nodes.size()+(*startNode!=Node())+(*endNode!=(Node())))));
    painter.drawText(width()-100,height()-10,QString("E: ").append(QString::number((edges.size()+startEdges.size()+endEdges.size()))));
}


/*********************************************************/
/*                      PATHFINDING                      */
/*********************************************************/


void Map::checkNeighbours(std::shared_ptr<Node> &currentNode) {
    for(auto& e:currentNode->getEdges()){
        int d=currentNode->getDistance()+e->getLength();
        if(e->getA()==*currentNode){
            if(!e->getB().isVisited() && d<e->getB().getDistance()){
                e->getB().setDistance(d);
                e->getB().setPredecessor(currentNode);
            }
        }
        else{
            if(!e->getB().isVisited() && d<e->getA().getDistance()){
                e->getA().setDistance(d);
                e->getA().setPredecessor(currentNode);
            }
        }
    }
}

std::shared_ptr<Node> Map::minDistNode(std::vector<std::shared_ptr<Node>> &tree){
    if(tree.size()>1) {
        return *std::min_element(tree.begin(),tree.end(),[](const std::shared_ptr<Node>& a,const std::shared_ptr<Node>& b){return a->getDistance()<b->getDistance();});
    }
    else{
        return tree[0];
    }
}
void Map::findPathDijkstra(){
    qDebug()<<"Searching path from"<<*startNode<<" to "<<*endNode<<" with Dijkstra";

    pathEdges.clear();
    pathNodes.clear();

    std::vector<std::shared_ptr<Node>> unvisited(nodes);
    unvisited.push_back(endNode);
    unvisited.push_back(startNode);

    QElapsedTimer timer;
    timer.start();

    for(auto& n:unvisited){
        n->setDistance(100000000);
        n->setNotVisited();
    }

    startNode->setDistance(0);

    int i=0;
    int j=0;
    while(!unvisited.empty()){
        std::shared_ptr<Node> currentNode=minDistNode(unvisited);
        qDebug()<<*currentNode;
        checkNeighbours(currentNode);
        currentNode->setVisited();
        if(*currentNode==*endNode){
            break;
        }
        auto itn=std::find(unvisited.begin(),unvisited.end(),currentNode);
        if(itn!=unvisited.end()){
            unvisited.erase(itn);
        }
    }
    qint64 end=timer.nsecsElapsed();
    qDebug()<<"Took "<<end/1000<<" µs";
    if(endNode->getDistance()==100000000){
        qDebug()<<"No Path found";
        return;
    }

    auto deb=qDebug();
    deb<<"Path: ";
    std::shared_ptr<Node> currentNode=endNode;
    while(*currentNode!=*startNode){
        deb<<*currentNode;
        pathNodes.push_back(currentNode);
        currentNode = currentNode->getPredecessor();
    }
    pathNodes.push_back(currentNode);
    qDebug()<<"Path length:"<<endNode->getDistance()<<" with "<<pathNodes.size()-2<<" nodes";

    deb<<*startNode;
    for(int i=0;i<pathNodes.size()-1;i++){
        pathEdges.push_back(std::make_shared<Edge>(Edge(pathNodes.at(i),pathNodes.at(i+1))));
    }
    update();
}