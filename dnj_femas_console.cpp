/*


*/

#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // strcmp
#include <string>
#include <sys/time.h> // gettimeofday

#include <dnj_femas_common.hpp>
#include <dnj_femas_console.hpp>


extern dnj_femas_global_setting dnj;

// 執行 console command (co) 的 pthread
void *read_console_in(void* data) {

    dnj.ttpReadconsolein = get_currentTT();
    //?? log to co.txt co_result.txt
    
    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    
    char sSource_co[10] = "co";
    char sCondition_co[10] = "co001";
    char cmd_stdin[50];
    
    while (1) {  
    
        read_stdin (cmd_stdin);
        
        if (strcmp(cmd_stdin, "i") == 0) {
            printf("tt: times \n");
            printf("j: ttJstart \n");
            printf("c: cfg content \n");
            printf("f: filenamee \n");
            printf("s: status \n");
            printf("0: load_cfg_init() \n"); // 還沒寫
            printf("1: flip isTGoLogin \n");
            printf("2: flip isMGoLogin \n");
            printf("3: flip isTauLogin \n");
            printf("4: flip isMauLogin \n");
            printf("5: flip isTGoOrder \n"); // 還沒寫
            printf("6: flip isMGoSub \n");
            printf("command: t1: tqq01 ReqUserLogin() \n");
            printf("command: t2: tqq02 ReqUserLogout() \n");
            printf("command: m1: mqq01 ReqUserLogin() \n");
            printf("command: m2: mqq02 ReqUserLogin() \n");
            printf("command: m4: mqq04 SubMarketData() \n");
            printf("command: m5: mqq05 UnSubMarketData() \n");
        }
        
        
        // just j related, as test
        if (strcmp(cmd_stdin, "j") == 0) {
            printf("ttJstart: \n");
            printf("timestamp:                    %s\n", dnj.ttJstart.timestamp);
            printf("tv.tv_sec:                    %d\n", dnj.ttJstart.tv.tv_sec);
            printf("tv.tv_usec:                             %d\n", dnj.ttJstart.tv.tv_usec);
            printf("timestamp_micro:              %s\n", dnj.ttJstart.timestamp_micro);
            printf("nRequestID:                       %s\n", dnj.ttJstart.nRequestID);
            printf("datentime_micro: %s\n", dnj.ttJstart.datentime_micro);
            printf("datentime:       %s\n", dnj.ttJstart.datentime);
            printf("date:            %s\n", dnj.ttJstart.date);
            printf("time:                       %s\n", dnj.ttJstart.time);
        } 
        // all connect time
        if (strcmp(cmd_stdin, "tt") == 0) {
            printf("ttJstart        : %s\n", dnj.ttJstart.datentime_micro      );
            printf("ttTstart        : %s\n", dnj.ttTstart.datentime_micro      );
            printf("ttMstart        : %s\n", dnj.ttMstart.datentime_micro      );
            printf("ttloadCFG       : %s\n", dnj.ttloadCFG.datentime_micro     );
            printf("ttpEchotime     : %s\n", dnj.ttpEchotime.datentime_micro   );
            printf("ttpReadintxt    : %s\n", dnj.ttpReadintxt.datentime_micro  );
            printf("ttpReadconsolein: %s\n", dnj.ttpReadconsolein.datentime_micro  );
            printf("ttTconnect      : %s\n", dnj.ttTconnect.datentime_micro    );
            printf("ttTlogin        : %s\n", dnj.ttTlogin.datentime_micro      );
            printf("ttMconnect      : %s\n", dnj.ttMconnect.datentime_micro    );
            printf("ttMlogin        : %s\n", dnj.ttMlogin.datentime_micro      );
            printf("ttTdisconnect   : %s\n", dnj.ttTdisconnect.datentime_micro );
            printf("ttTlogout       : %s\n", dnj.ttTlogout.datentime_micro     );
            printf("ttMdisconnect   : %s\n", dnj.ttMdisconnect.datentime_micro );
            printf("ttMlogout       : %s\n", dnj.ttMlogout.datentime_micro     );
        } 
        
        // c: cfg content
        if (strcmp(cmd_stdin, "c") == 0) {
            printf("basenamee  : %s\n", dnj.basenamee   );
            printf("gBrokerID  : %s\n", dnj.gBrokerID   );
            printf("gUserID    : %s\n", dnj.gUserID     );
            printf("gInvestorID: %s\n", dnj.gInvestorID );
            printf("gPassword  : %s\n", dnj.gPassword   );
            printf("gExchangeID: %s\n", dnj.gExchangeID );
            printf("gClientID  : %s\n", dnj.gClientID   );
            printf("gMAddr     : %s\n", dnj.gMAddr      );
            printf("gTAddr     : %s\n", dnj.gTAddr      );
        }
        // f: filenamee
        if (strcmp(cmd_stdin, "f") == 0) {
            printf("basenamee             : %s\n", dnj.basenamee             ); // /home/jie/c_data
            printf("datenamee             : %s\n", dnj.datenamee             ); // 20190403
            printf("pathnamee             : %s\n", dnj.pathnamee             ); // /home/jie/c_data/20190403
            printf("filenamee_co          : %s\n", dnj.filenamee_co          ); // co.txt
            printf("filenamee_in          : %s\n", dnj.filenamee_in          ); // in.txt
            printf("filenamee_co_result   : %s\n", dnj.filenamee_co_result   ); // irr.txt (co in 放在同一個)
            printf("filenamee_in_result   : %s\n", dnj.filenamee_in_result   ); // irr.txt (co in 放在同一個)
            printf("filenamee_rr_rsp      : %s\n", dnj.filenamee_rr_rsp      ); // rr_rsp.txt
            printf("filenamee_rr_rtn      : %s\n", dnj.filenamee_rr_rtn      ); // rr_rtn.txt
            printf("filenamee_rr_rtn_trr22: %s\n", dnj.filenamee_rr_rtn_trr22); // rr_rtn.txt (暫時先設定 rr_rtn.txt) trr22 OnRtnOrder() 报单回报及成交
            printf("filenamee_rr_rtn_mrr11: %s\n", dnj.filenamee_rr_rtn_mrr11); // rr_rtn_mrr11.txt mrr11 OnRtnDepthMarketData() 深度行情通知
        }
        // those bool
        if (strcmp(cmd_stdin, "s") == 0) {
            printf("isJlogged   : %d\n", dnj.isJlogged );
            printf("isTConnectOK: %d\n", dnj.isTConnectOK );
            printf("isMConnectOK: %d\n", dnj.isMConnectOK );
            printf("isTLoginOK  : %d\n", dnj.isTLoginOK   );
            printf("isMLoginOK  : %d\n", dnj.isMLoginOK   );
            printf("isMmrr11OK  : %d\n", dnj.isMReadContractsOK   );
            printf("iMP         = %d\n", dnj.iMP   );
            printf("1:isTGoLogin: %d\n", dnj.isTGoLogin   );
            printf("2:isMGoLogin: %d\n", dnj.isMGoLogin   );
            printf("3:isTauLogin: %d\n", dnj.isTauLogin   );
            printf("4:isMauLogin: %d\n", dnj.isMauLogin   );
            printf("5:isTGoOrder: %d\n", dnj.isTGoOrder   );
            printf("6:isMGoSub:   %d\n", dnj.isMGoSub   );
        }
        // flip cqq001
        if (strcmp(cmd_stdin, "1") == 0) {
            printf("isTGoLogin  : %d -> ", dnj.isTGoLogin   );
            if (dnj.isTGoLogin == true) {
                dnj.isTGoLogin = false;
            } else {
                dnj.isTGoLogin = true;
            }
            printf("%d\n", dnj.isTGoLogin   );
        }
        // flip cqq002
        if (strcmp(cmd_stdin, "2") == 0) {
            printf("isMGoLogin  : %d -> ", dnj.isMGoLogin   );
            if (dnj.isMGoLogin == true) {
                dnj.isMGoLogin = false;
            } else {
                dnj.isMGoLogin = true;
            }
            printf("%d\n", dnj.isMGoLogin   );
        }
        // flip 
        if (strcmp(cmd_stdin, "3") == 0) {
            printf("isTauLogin  : %d -> ", dnj.isTauLogin   );
            if (dnj.isTauLogin == true) {
                dnj.isTauLogin = false;
            } else {
                dnj.isTauLogin = true;
            }
            printf("%d\n", dnj.isTauLogin   );
        }
        // flip 
        if (strcmp(cmd_stdin, "4") == 0) {
            printf("isMauLogin  : %d -> ", dnj.isMauLogin   );
            if (dnj.isMauLogin == true) {
                dnj.isMauLogin = false;
            } else {
                dnj.isMauLogin = true;
            }
            printf("%d\n", dnj.isMauLogin   );
        }
        
        
        
        // flip cqq005
        if (strcmp(cmd_stdin, "5") == 0) {
            printf("isTGoOrder  : %d -> ", dnj.isTGoOrder   );
            if (dnj.isTGoOrder == true) {
                dnj.isTGoOrder = false;
            } else {
                dnj.isTGoOrder = true;
            }
            printf("%d\n", dnj.isTGoOrder   );
        } 
        // flip cqq006
        if (strcmp(cmd_stdin, "6") == 0) {
            printf("isMGoSub  : %d -> ", dnj.isMGoSub   );
            if (dnj.isMGoSub == true) {
                dnj.isMGoSub = false;
            } else {
                dnj.isMGoSub = true;
            }
            printf("%d\n", dnj.isMGoSub   );
        } 

        if (strcmp(cmd_stdin, "0") == 0) {
            
            // log?
            pthread_cancel(*dnj.t_read_in_txt);
            
            // reload cfg
            dnj.load_cfg_init(sSource_co, sCondition_co);

            // log?
            pthread_t tt_read_in_txt; // 宣告 pthread 變數
            pthread_create(&tt_read_in_txt, NULL, read_in_txt, NULL); // 建立子執行緒
        //    pthread_create(dnj.t_read_in_txt, NULL, read_in_txt, NULL); // 建立子執行緒 (這樣會 crash)
            dnj.t_read_in_txt = &tt_read_in_txt;
        }
        
        // 其他指令
        do_console_in(cmd_stdin, sSource_co, sCondition_co);

    } // end of while (1)
    
}


void do_console_in(char* sCmd_this, char* sSource_this, char* sCondition_this) {

        if (strcmp(sCmd_this, "t1") == 0) {
            // do T login
            ///tqq01: ReqUserLogin() 风控前置系统用户登录请求
            console_in_tqq01(sSource_this, sCondition_this);
        }
        if (strcmp(sCmd_this, "t2") == 0) {
            // do T logout
            ///tqq02:ReqUserLogout() 用户退出请求
            console_in_tqq02(sSource_this, sCondition_this);
        }
        if (strcmp(sCmd_this, "m1") == 0) {
            // do M login
            ///mqq01: ReqUserLogin() 风控前置系统用户登录请求
            console_in_mqq01(sSource_this, sCondition_this);
        }
        if (strcmp(sCmd_this, "m2") == 0) {
            // do M logout
            ///mqq02: ReqUserLogout() 用户退出请求
            console_in_mqq02(sSource_this, sCondition_this);
        }
        if (strcmp(sCmd_this, "m4") == 0) {
            // do M Sub
            ///mqq04: SubMarketData() 订阅合约行情, 預定 IF* 和 IO*
            console_in_mqq04(sSource_this, sCondition_this);
        }
        if (strcmp(sCmd_this, "m5") == 0) {
            // do M UnSub
            ///mqq05: UnSubMarketData() 退订合约行情, 預定 * 全部退訂
            console_in_mqq05(sSource_this, sCondition_this);
        }


}

// 這個 function 好像沒有作用
void console_in_mqq05(char* sSource_this, char* sCondition_this) {
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    // do M UnSub
    ///mqq05: UnSubMarketData() 退订合约行情, 預定 * 全部退訂
    
    // 接受全部 用1個 * 即可
    char *contracts[1];
    contracts[0] = (char *) "*"; // 可以
    pMUserApi -> UnSubMarketData(contracts, 1); 

    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "mqq05");   
    strcpy(cFields_co[3], "UnSubMarketData()");  
    strcpy(cFields_co[4], "mcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], "1");
    strcpy(cFields_co[21], contracts[0]);
    do_sline_co(cFields_co, 22);
    int iRR = pMUserApi -> UnSubMarketData(contracts, 1);
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 22);
    printf("%s,mqq05,UnSubMarketData(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}
void console_in_mqq04(char* sSource_this, char* sCondition_this) {
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    // do M sub
    // mqq04: SubMarketData() 订阅合约行情, 預定 IF* 和 IO*
    /*
    // 订阅 /home/jie/c_data/contract_data.txt
    char *contracts[500]; // = {"","",""};
    int iContractCnt = 0;
    iContractCnt = read_instruments(contracts, basenamee_tmp);
    pMUserApi -> SubMarketData(contracts, iContractCnt);
    */
    /*
    // 接受全部 订阅: Sub 返回订阅合 用1個 * 即可
    char *contracts[1];
    contracts[0] = (char *) "*"; // 可以
    //pMUserApi -> SubMarketData(contracts, 1); 
    */
    
    char *contracts[2];
    contracts[0] = (char *) "IO*"; // 可以
    contracts[1] = (char *) "IF*"; // 可以
    
    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "mqq04");  
    strcpy(cFields_co[3], "SubMarketData()");  
    strcpy(cFields_co[4], "mcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], "2");
    strcpy(cFields_co[21], contracts[0]);
    strcpy(cFields_co[22], contracts[1]);
    do_sline_co(cFields_co, 23);
//    int iRR = pMUserApi -> SubMarketData(contracts, 1);
    int iRR = pMUserApi -> SubMarketData(contracts, 2);
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 23);
    printf("%s,mqq04,SubMarketData(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}
void console_in_mqq02(char* sSource_this, char* sCondition_this) {
//    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    // do M logout
    ///mqq02: ReqUserLogout() 用户退出请求
    CUstpFtdcReqUserLogoutField reqLogout;
    memset(&reqLogout, 0, sizeof(reqLogout));
    strcpy(reqLogout.BrokerID, dnj.gBrokerID);
    strcpy(reqLogout.UserID, dnj.gUserID);
    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "mqq02");   
    strcpy(cFields_co[3], "ReqUserLogout()");  
    strcpy(cFields_co[4], "mcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], reqLogout.BrokerID);
    strcpy(cFields_co[21], reqLogout.UserID);
    do_sline_co(cFields_co, 22);
    int iRR = pMUserApi -> ReqUserLogout(&reqLogout, iRequestID); // 发出登陆请求
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 22);
    printf("%s,mqq02,ReqUserLogout(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}
void console_in_mqq01(char* sSource_this, char* sCondition_this) {
//    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    // do M login
    ///mqq01: ReqUserLogin() 风控前置系统用户登录请求
    CUstpFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin, 0, sizeof(reqUserLogin));
    //strcpy(reqUserLogin.TradingDay, tt_co.date8);
    strcpy(reqUserLogin.BrokerID, dnj.gBrokerID);
    strcpy(reqUserLogin.UserID, dnj.gUserID);
    strcpy(reqUserLogin.Password, dnj.gPassword);
    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "mqq01");   
    strcpy(cFields_co[3], "ReqUserLogin()");  
    strcpy(cFields_co[4], "mcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], reqUserLogin.BrokerID);
    strcpy(cFields_co[21], reqUserLogin.UserID);
    strcpy(cFields_co[22], reqUserLogin.Password);
    do_sline_co(cFields_co, 23);
    int iRR = pMUserApi -> ReqUserLogin(&reqUserLogin, iRequestID); // 发出登陆请求
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 23);
    printf("%s,mqq01,ReqUserLogin(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}
void console_in_tqq02(char* sSource_this, char* sCondition_this) {
    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
//    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    // do T logout
    ///tqq02:ReqUserLogout() 用户退出请求
    CUstpFtdcReqUserLogoutField reqLogout;
    memset(&reqLogout, 0, sizeof(reqLogout));
    strcpy(reqLogout.BrokerID, dnj.gBrokerID);
    strcpy(reqLogout.UserID, dnj.gUserID);
    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "tqq02");   
    strcpy(cFields_co[3], "ReqUserLogout()");  
    strcpy(cFields_co[4], "tcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], reqLogout.BrokerID);
    strcpy(cFields_co[21], reqLogout.UserID);
    do_sline_co(cFields_co, 22);
    int iRR = pTUserApi -> ReqUserLogout(&reqLogout, iRequestID); // 发出登陆请求
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 22);
    printf("%s,tqq02,ReqUserLogout(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}
void console_in_tqq01(char* sSource_this, char* sCondition_this) {
    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
    ///tqq01: ReqUserLogin() 风控前置系统用户登录请求
    CUstpFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin, 0, sizeof(reqUserLogin));
    // strcpy(reqUserLogin.TradingDay, pTUserApi -> GetTradingDay()); //wrong date
    //strcpy(reqUserLogin.TradingDay, tt_co.date8);
    strcpy(reqUserLogin.BrokerID, dnj.gBrokerID);
    strcpy(reqUserLogin.UserID, dnj.gUserID);
    strcpy(reqUserLogin.Password, dnj.gPassword);
    ttTime tt_co = get_currentTT();
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "tqq01");   
    strcpy(cFields_co[3], "ReqUserLogin()");  
    strcpy(cFields_co[4], "tcmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   //
    // data start from [#20], 21st fields    
    strcpy(cFields_co[20], reqUserLogin.BrokerID);
    strcpy(cFields_co[21], reqUserLogin.UserID);
    strcpy(cFields_co[22], reqUserLogin.Password);
    do_sline_co(cFields_co, 23);
    int iRR = pTUserApi -> ReqUserLogin(&reqUserLogin, iRequestID); // 发出登陆请求
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 23);
    printf("%s,tqq01,ReqUserLogin(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 
}

// co co_irr 不是很需要 persistent
void do_sline_co(char* cFields[], int cnt_cFields) {
    char sline_co[300] = "";
    sprintf(sline_co,       "%s", cFields[0]);
    for (int idx=1; idx<cnt_cFields; idx++) {
        sprintf(sline_co,"%s,%s", sline_co, cFields[idx]); 
    }
    append_file(dnj.filenamee_co, sline_co);
}
void do_sline_co_result(char* cFields[], int cnt_cFields) {
    char sline_co_result[300] = "";
    sprintf(sline_co_result,       "%s", cFields[0]);
    for (int idx=1; idx<cnt_cFields; idx++) {
        sprintf(sline_co_result,"%s,%s", sline_co_result, cFields[idx]); 
    }
    append_file(dnj.filenamee_co_result, sline_co_result);
}


