//
// Created by tic-tac on 11/01/18.
//

#include <iostream>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : menu(new Menu(this)), map(new Map(this,20)) {
    connect(menu->btnReset, &QPushButton::clicked,map, &Map::reset);
    connect(menu->btnStart, &QPushButton::clicked,map, &Map::start);
    connect(menu->btnMove, &QPushButton::clicked,map, &Map::mapMovement);
    connect(menu->btnMove, &QPushButton::clicked,this, &MainWindow::mapMovement);
    connect(menu->btnPath, &QPushButton::clicked,map, &Map::findPathDijkstra);

    map->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    installEventFilter(this);
    QVBoxLayout* lay=new QVBoxLayout(this);
    lay->setMargin(0);
    lay->addWidget(menu);
    lay->addWidget(map);
    setLayout(lay);
}

void MainWindow::mapMovement(){
    menu->btnMove->setText(menu->btnMove->text().compare("Lancer")?"Lancer":"Pause");
    menu->update();
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type()==QEvent::KeyPress){
        QKeyEvent* keyEvent= static_cast<QKeyEvent*>(event);
        if(keyEvent->key()==Qt::Key_Escape){
            std::cout<<"Escape pressed, exiting"<<std::endl;
            this->close();
        }
    }
    return QObject::eventFilter(obj,event);
}