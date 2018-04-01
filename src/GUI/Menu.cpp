//
// Created by tic-tac on 30/11/17.
//

#include "Menu.h"

Menu::Menu(QWidget *parent):QWidget(parent){
    btnStart = new QPushButton("Genérer", this);
    btnReset = new QPushButton("Effacer", this);
    btnMove = new QPushButton("Lancer", this);
    btnQuit = new QPushButton("Quitter",this);
    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(btnReset, 0,0);
    grid->addWidget(btnStart, 1,0);
    grid->addWidget(btnQuit,0,1);
    grid->addWidget(btnMove, 1,1);

    setLayout(grid);
}

void Menu::paintEvent(QPaintEvent *event) {
}