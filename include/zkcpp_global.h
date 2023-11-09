#ifndef __ZKCPP_GBLDEF_H__
#define __ZKCPP_GBLDEF_H__

#include <signal.h>
#include <stdio.h>
#include <stddef.h>

typedef struct
{
	int  log_level;			//日志级别 或者日志类型，zkcpp_macro.h里分0-8共9个级别
	int  fd;				//日志文件描述符

}zkcpp_log_t;


//结构定义,用来储存有效配置信息
typedef struct _CConfItem
{
	char ItemName[50];      //配置信息名
	char ItemContent[500];  //配置信息数据
}CConfItem, *LPCConfItem;

//数据变量类型相关
typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned int	u_int;
typedef unsigned long	u_long;
typedef int				pid_t;

//外部全局量声明
extern zkcpp_log_t	 zkcpp_log;
#endif