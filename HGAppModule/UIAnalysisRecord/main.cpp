#include "hganalysisrecordwidget.h"
#include "common.h"
#include <iostream>
#include <QApplication>
#include "SvcFactory.h"


int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        SvcFactory::CreateFrameService()->loadConfig(basePath.toStdString());
        SvcFactory::CreateFrameService()->saveConfig();
        HGAnalysisRecordWidget w("zh");
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}