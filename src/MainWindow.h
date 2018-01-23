//
// Created by tic-tac on 11/01/18.
//

#ifndef QTSTAR_MAINWINDOW_H
#define QTSTAR_MAINWINDOW_H


#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include "Menu.h"
#include "Graph/Map.h"

class MainWindow : public QWidget{
public:
    MainWindow(QWidget* parent=0);

    Menu* menu;
    Map* map;

    bool eventFilter(QObject *obj, QEvent *event);

    void mapMovement();
};


#endif //QTSTAR_MAINWINDOW_H
