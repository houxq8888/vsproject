#ifndef SVC_FACTORY_H
#define SVC_FACTORY_H

#include "IBaseCommon.h"
#include "IBaseConfig.h"
#include "ISvcSave.h"
#include "ISvcLog.h"
#include "ISvcFrame.h"
#include "ITime.h"
#include "IDatabaseManager.h"
#include <memory>

class SvcFactory {
public:
    static std::shared_ptr<IBaseCommon> CreateCommonService();
    static std::shared_ptr<IBaseConfig> CreateConfigService();
    static std::shared_ptr<ISvcSave> CreateSaveService();
    static std::shared_ptr<ISvcLog> CreateLogService();
    static std::shared_ptr<ISvcFrame> CreateFrameService();
    static std::shared_ptr<ITime> CreateTimeService();
    static std::shared_ptr<IDatabaseManager> CreateDatabaseManager();
};

#endif // SVC_FACTORY_H
