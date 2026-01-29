#include "hgdisplaychartwidget.h"
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
        
        std::vector<std::string> names={
            "时间~电压","水质量~电压","时间~水质量","时间~电解速率"
        };
        HGDisplayChartWidget w("zh",names);
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}