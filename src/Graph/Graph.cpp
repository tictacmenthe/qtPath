//
// Created by tic-tac on 4/1/18.
//

#include <QtCore/QElapsedTimer>
#include "Graph.h"

Graph::Graph(int p_nbCircles, int p_width, int p_height, QRect parentRect):parent(parentRect) {

    nbCircles=p_nbCircles;
    width=p_width;
    height=p_height;
    startNode=std::make_shared<Node>();
    endNode=std::make_shared<Node>();
}


/****************************************************
 *                      Generating                  *
 ****************************************************/

void Graph::populate() {
    clear();

    nodes.push_back(std::shared_ptr<Node>(new Node(1,1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(1,height-1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(width-1,1)));
    nodes.push_back(std::shared_ptr<Node>(new Node(width-1,height-1)));

    populateObstacles();
    populateNodes();
    populateEdges();
}

void Graph::populateObstacles(){
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<float> sizeDist(10,height/8+20);
    for(int i=0;i<nbCircles;){
        int size=abs(sizeDist(generator))+10;
        std::uniform_int_distribution<int> xDist(size,width-size);
        std::uniform_int_distribution<int> yDist(size,height-size);
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

void Graph::populateNodes() {
    for(const Circle& c:obstacles){
        int nbNodes=sqrt(c.getRadius());
        for(int i=0;i<nbNodes;i++){
            std::shared_ptr<Node> position=std::make_shared<Node>(c.getCenter()+Vec((float)(i*2*3.14/nbNodes), (float)(c.getRadius()*1.05+10)));
            addNode(position);
        }
    }
}

void Graph::populateEdges(){
    int nbNodes=nodes.size();
    for(int i=0;i<nbNodes-1;i++) {
        for (int j = i + 1; j < nbNodes; j++) {
            addEdge(nodes.at(i), nodes.at(j));
        }
    }
}


void Graph::clear(){
    obstacles.clear();
    nodes.clear();
    edges.clear();

    startNode=std::make_shared<Node>();
    startEdges.clear();

    endNode=std::make_shared<Node>();
    endEdges.clear();

    pathNodes.clear();
    pathEdges.clear();
}


/****************************************************
 *                      Adding                      *
 ****************************************************/

void Graph::addCircle(const Vec &center, int radius) {
    obstacles.emplace_back(center, radius);
}


void Graph::addNode(std::shared_ptr<Node>& position, bool isStart, bool isEnd){
    bool add=parent.contains(*position);
    for(const Circle& c:obstacles){
        if(c.contains(*position)){
            add=false;
            break;
        }
    }
    for(const auto& n:nodes){
        Circle minDistCircle(*n,0);
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

void Graph::addEdge(std::shared_ptr<Node> &a,std::shared_ptr<Node> &b, bool isStart, bool isEnd) {
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


/****************************************************
 *                      Removing                    *
 ****************************************************/

int Graph::checkObstacleInEdge(const Circle &c, const Edge &e, bool verifyDistance) {
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

void Graph::removeEdgesOfNode(Node &n){
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

/****************************************************
 *                      GUI                         *
 ****************************************************/
void Graph::paint(QPainter &painter) {
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

    painter.setPen(QPen(Qt::yellow,0.05));
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

    painter.drawText(width-100,height-50,QString("C: ").append(QString::number(obstacles.size())));
    painter.drawText(width-100,height-30,QString("N: ").append(QString::number(nodes.size()+(*startNode!=Node())+(*endNode!=(Node())))));
    painter.drawText(width-100,height-10,QString("E: ").append(QString::number((edges.size()+startEdges.size()+endEdges.size()))));
}


/****************************************************
 *                      PathFinding                 *
 ****************************************************/

void Graph::checkNeighbours(std::shared_ptr<Node> &currentNode) {
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

std::shared_ptr<Node> Graph::minDistNode(std::vector<std::shared_ptr<Node>> &tree){
    if(tree.size()>1) {
        std::vector<std::shared_ptr<Node>>::iterator node=std::min_element(tree.begin(),tree.end(),
                                                                           [](const std::shared_ptr<Node>& a,const std::shared_ptr<Node>& b)
                                                                           {
                                                                               return a->getDistance()<b->getDistance();
                                                                           }
        );
        while((*node)->isVisited()){
            if(node!=tree.end()){
                tree.erase(node);
            }
            node=std::min_element(tree.begin(),tree.end(),
                                  [](const std::shared_ptr<Node>& a,const std::shared_ptr<Node>& b)
                                  {
                                      return a->getDistance()<b->getDistance();
                                  }
            );
        }
        return *node;
    }
    else{
        return tree[0];
    }
}

void Graph::findPathDijkstra(){
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

    while(!unvisited.empty()){
        std::shared_ptr<Node> currentNode=minDistNode(unvisited);
        checkNeighbours(currentNode);
        currentNode->setVisited();
        if(*currentNode==*endNode){
            break;
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

}


/****************************************************
 *                      Animation                   *
 ****************************************************/

void Graph::update() {
    std::default_random_engine generator(time(nullptr));
    std::normal_distribution<float> distribution(0,0.2);

    for(Circle& c: obstacles){
        float addvx=distribution(generator);
        float addvy=distribution(generator);
        c.addSpeeds(addvx,addvy);
        c.setCenter(c.getCenter()+Vec(c.getSpeedX(),c.getSpeedY()));
        if(!parent.contains(c.getCenter())){
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
        if(!parent.contains(n) || toErase){
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
}

