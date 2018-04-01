//
// Created by tic-tac on 11/01/18.
//

#ifndef QTSTAR_MAINWINDOW_H
#define QTSTAR_MAINWINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QPainter>
#include "Menu.h"
#include "MapFrame.h"

class MainWindow : public QWidget{
public:
    MainWindow(QWidget* parent=0);
    virtual ~MainWindow(){
        delete menu;
        delete map;
    }
    Menu* menu;
    MapFrame* map;
    bool eventFilter(QObject *obj, QEvent *event);
    void mapMovement();
};

#endif //QTSTAR_MAINWINDOW_H
