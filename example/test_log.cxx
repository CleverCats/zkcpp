#include<iostream>
#include "zkcpp_func.h"
#include "zkcpp_conf.h"
#include "zkcpp_macro.h"
int main(int argc, char *argv[])
{
    CConfig *config_instance = CConfig::GetInstance();
    config_instance->Load("../zkcpp.conf");
    zkcpp_log_init();
    zkcpp_log_error_core(ZKCPP_LOG_STDERR, 0, "日志输出测试: %s ...","测试成功");
    return 0;
}