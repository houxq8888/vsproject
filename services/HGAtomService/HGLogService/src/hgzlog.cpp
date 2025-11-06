#include "hgzlog.h"

namespace HGMACHINE {

HGLogService::HGLogService()
{

}
HGLogService::~HGLogService()
{

}

void HGLogService::logInit()
{
    int rc;
//    rc = zlog_init(LOG_CONF); // LOG_CONF
//    if (rc)
//    {
//        printf("[HG]: Log WARNING!!!!! init failed\n");
//        return ;
//    }

//    gp_VEHService = zlog_get_category("HG");
//    if (!gp_VEHService)
//    {
//        printf("[HG]: Log WARNING!!!!! get HG fail\n");
//        zlog_fini();
//        return ;
//    }
}

void HGLogService::logDeInit(void)
{
    zlog_fini();
}

}
