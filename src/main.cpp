#include <QtWidgets/QApplication>
#include "GUI/MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("QtStar");
    window.setMinimumSize(640,480);
    window.showMaximized();
    window.show();
    int error=app.exec();
    return error;
}
