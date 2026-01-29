#include "hgprintwidget.h"
#include <QApplication>
#include <iostream>
#include "common.h"
#include "SvcFactory.h"

int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        SvcFactory::CreateFrameService()->loadConfig(basePath.toStdString());
        SvcFactory::CreateFrameService()->saveConfig();
        
        std::vector<std::string> printNames={"PrintName","PrintData","PrintTime","PrintUser","PrintIP","PrintMac","PrintModel",\
            "PrintSerialNumber","PrintSoftwareVersion","PrintHardwareVersion","PrintStatus","PrintResult","PrintRemark","PrintSignature",\
            "PrintSignatureTime","PrintSignatureUser","PrintSignatureIP","PrintSignatureMac","PrintSignatureModel","PrintSignatureSerialNumber",\
            "PrintSignatureSoftwareVersion","PrintHardwareVersion","PrintSignatureHardwareVersion","PrintSignatureStatus"};
    
        HGPrintWidget w("zh","Report",printNames);
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}