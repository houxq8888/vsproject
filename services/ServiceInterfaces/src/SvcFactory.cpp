#include "SvcFactory.h"
#include "IBaseCommon.h"
#include "IBaseConfig.h"
#include "ISvcLog.h"
#include "ISvcSave.h"
#include "ISvcFrame.h"
#include "ITime.h"
#include "IDatabaseManager.h"
#include "BaseCommonAdapter.h"
#include "BaseConfigAdapter.h"
#include "SvcLogAdapter.h"
#include "SvcSaveAdapter.h"
#include "SvcFrameAdapter.h"
#include "TimeAdapter.h"
#include "DatabaseManagerAdapter.h"

std::shared_ptr<IBaseCommon> SvcFactory::CreateCommonService() {
    return std::make_shared<BaseCommonAdapter>();
}

std::shared_ptr<IBaseConfig> SvcFactory::CreateConfigService() {
    return std::make_shared<BaseConfigAdapter>();
}

std::shared_ptr<ISvcLog> SvcFactory::CreateLogService() {
    return std::make_shared<SvcLogAdapter>();
}

std::shared_ptr<ISvcSave> SvcFactory::CreateSaveService() {
    return std::make_shared<SvcSaveAdapter>();
}

std::shared_ptr<ISvcFrame> SvcFactory::CreateFrameService() {
    return std::make_shared<SvcFrameAdapter>();
}

std::shared_ptr<ITime> SvcFactory::CreateTimeService() {
    return std::make_shared<TimeAdapter>();
}

std::shared_ptr<IDatabaseManager> SvcFactory::CreateDatabaseManager() {
    static std::shared_ptr<IDatabaseManager> instance = std::make_shared<DatabaseManagerAdapter>();
    return instance;
}
