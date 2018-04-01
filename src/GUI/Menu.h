//
// Created by tic-tac on 30/11/17.
//
#ifndef QTSTAR_MENU_H
#define QTSTAR_MENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QPainter>

class Menu : public QWidget {
public:
    Menu(QWidget* parent=0);
    void paintEvent(QPaintEvent *event) override;

    QPushButton* btnReset;
    QPushButton* btnMove;
    QPushButton* btnStart;
    QPushButton* btnQuit;
};

#endif //QTSTAR_MENU_H
