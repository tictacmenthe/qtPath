#include <QtWidgets/QApplication>
#include "GUI/MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("QtStar");
    window.setMinimumSize(640,480);
    window.showMaximized();
    window.show();

    //Dark Theme
    QFile file("../stylesheet/dark.qss");
    if(!file.exists()){
        qDebug()<<"File "<<file.fileName()<<" not found !";
        return 1;
    }

    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream stream(&file);

    app.setStyleSheet(stream.readAll());

    int error=app.exec();
    return error;
}
