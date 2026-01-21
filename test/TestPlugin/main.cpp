#include "testwidget.h"
#include <QApplication>
#include <iostream>



int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        TestWidget w;
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}