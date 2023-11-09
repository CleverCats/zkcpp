#ifndef __ZKCPP_FUNC_H__
#define __ZKCPP_FUNC_H__
#include "zkcpp_global.h"
void zkcpp_log_init();									 //初始化日志(从配置文件读取日志相关信息)
void zkcpp_log_stderr(int err, const char *fmt, ...);		 //标准错误日志打印(打印到屏幕 + 文件)
void zkcpp_log_error_core(int level, int err, const char *fmt, ...); //打印文件日志(打印到文件)

u_char *zkcpp_log_errno(u_char *buf, u_char *last, int err);//打印错误提示
u_char *zkcpp_vslprintf(u_char *buf, u_char *last, const char *fmt, va_list args);
u_char *zkcpp_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char *zkcpp_slprintf(u_char *buf, u_char *last, const char *fmt, ...);
#endif  