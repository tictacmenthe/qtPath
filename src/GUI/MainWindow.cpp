//
// Created by tic-tac on 11/01/18.
//

#include <iostream>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : menu(new Menu(this)), map(new MapFrame(this,25)) {
    connect(menu->btnReset, &QPushButton::clicked,map, &MapFrame::reset);
    connect(menu->btnStart, &QPushButton::clicked,map, &MapFrame::start);
    connect(menu->btnMove, &QPushButton::clicked,this, &MainWindow::mapMovement);
    connect(menu->btnQuit, &QPushButton::clicked,this, &MainWindow::close);

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
    map->mapMovement();
    menu->update();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type()==QEvent::KeyPress){
        QKeyEvent* keyEvent= static_cast<QKeyEvent*>(event);
        if(keyEvent->key()==Qt::Key_Escape){
            std::cout<<"Escape pressed, exiting"<<std::endl;
            this->close();
        }
        else if(keyEvent->key()==Qt::Key_Space){
            static bool active=false;
            std::cout<<"Space pressed, generating"<<std::endl;
            if(!active){
                map->start();
                active=true;
            }
            else{
                map->reset();
                active=false;
            }
        }
    }
    return QObject::eventFilter(obj,event);
}