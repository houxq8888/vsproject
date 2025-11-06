#include "hgloginwidget.h"
#include <QApplication>
#include <iostream>
#include "common.h"

int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        std::string dbDir = basePath.toStdString() + "/database";
        HGMkDir(dbDir);
        loadConfig(basePath.toStdString());
        saveConfig();

        HGLoginWidget w("zh");
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}