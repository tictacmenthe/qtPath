//
// Created by tic-tac on 30/11/17.
//
#ifndef QTSTAR_MENU_H
#define QTSTAR_MENU_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>

class Menu : public QWidget {
    Q_OBJECT
public:
    Menu(QWidget* parent=0);
    void paintEvent(QPaintEvent *event) override;

    QPushButton* btnReset;
    QPushButton* btnMove;
    QPushButton* btnStart;
    QPushButton* btnPath;
};

#endif //QTSTAR_MENU_H
