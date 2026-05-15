#include "hgsystemsetwidget.h"
#include "SvcFactory.h"



int main(int argc, char *argv[]){
    try {
        QApplication app(argc, argv);
        QString basePath = qApp->applicationDirPath();
        auto frameService = SvcFactory::CreateFrameService();
        frameService->loadConfig(basePath.toStdString());
        frameService->saveConfig();

        std::vector<std::string> names={
            "SampleDetection",
            "SampleEditing",
            "ManualControl",
            "CreateMethod",
            "RectifyMethod",
            "CopyMethod",
            "DeleteMethod",
            "DeviceManage",
            "Recalculation",
            "ReportEditing",
            "PrintFunction",
            "RecordOutput",
            "UserManage(Manager)",
            "UserManage(Users)",
            "AuthorityManage",
            "AuditTrail",
            "TimeSetting",
            "DeviceInfo",
            "ScanAuditTrailRecord",
            "SampleStatisticsSettings"
        };
        AuthorityWidget w("zh",names);
        w.show();
        return app.exec();
    } catch (const std::exception& e){
        std::cerr<<"Exception caught:" << e.what()<<std::endl;
        return 1;
    }
}