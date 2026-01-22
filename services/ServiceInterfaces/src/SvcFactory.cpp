#include "SvcFactory.h"
#include "IBaseCommon.h"
#include "IBaseConfig.h"
#include "ISvcLog.h"
#include "ISvcSave.h"
#include "ISvcFrame.h"
#include "ITime.h"
#include "IUserDataAccess.h"
#include "ISystemDataAccess.h"
#include "IAuthorityDataAccess.h"
#include "IDataChartDataAccess.h"
#include "BaseCommonAdapter.h"
#include "BaseConfigAdapter.h"
#include "SvcLogAdapter.h"
#include "SvcSaveAdapter.h"
#include "SvcFrameAdapter.h"
#include "TimeAdapter.h"
#include "UserDataAccessAdapter.h"
#include "SystemDataAccessAdapter.h"
#include "AuthorityDataAccessAdapter.h"
#include "DataChartDataAccessAdapter.h"

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

std::shared_ptr<IUserDataAccess> SvcFactory::CreateUserDataAccess() {
    return std::make_shared<UserDataAccessAdapter>();
}

std::shared_ptr<ISystemDataAccess> SvcFactory::CreateSystemDataAccess() {
    return std::make_shared<SystemDataAccessAdapter>();
}

std::shared_ptr<IAuthorityDataAccess> SvcFactory::CreateAuthorityDataAccess() {
    return std::make_shared<AuthorityDataAccessAdapter>();
}

std::shared_ptr<IDataChartDataAccess> SvcFactory::CreateDataChartDataAccess() {
    return std::make_shared<DataChartDataAccessAdapter>();
}
}
