/*


*/

#include "USTPFtdcTraderApi.h"
#include "USTPFtdcMduserApi.h"
void do_sline_co(char* cFields[], int cnt_cFields);
void do_sline_co_result(char* cFields[], int iRR);
void *read_console_in(void*);
void do_console_in(char* sCmd_this, char* sSource_this, char* sCondition_this); 
void console_in_tqq01(char* sSource_this, char* sCondition_this); ///tqq01: ReqUserLogin() 风控前置系统用户登录请求
void console_in_tqq02(char* sSource_this, char* sCondition_this); ///tqq02: ReqUserLogout() 用户退出请求
void console_in_mqq01(char* sSource_this, char* sCondition_this); ///mqq01: ReqUserLogin() 风控前置系统用户登录请求
void console_in_mqq02(char* sSource_this, char* sCondition_this); ///mqq02: ReqUserLogout() 用户退出请求
void console_in_mqq04(char* sSource_this, char* sCondition_this); ///mqq04: SubMarketData() 订阅合约行情, 預定 IF* 和 IO*
void console_in_mqq05(char* sSource_this, char* sCondition_this); ///mqq05: UnSubMarketData() 退订合约行情, 預定 * 全部退訂
