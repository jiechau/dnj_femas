/*
usage:
tail -n1 -f /home/jie/c_data/20190318/in.txt | ./ttest2_ReqRspRtn_in.out

on the other hand: the input file is in:
cd /home/jie/c_data/20190318
cat /dev/null > /home/jie/c_data/20190318/in.txt
python3 ~/cffex/api_af/demo_python/ptest1_qq04_order_cmd.py

*/
#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // strcmp
#include <errno.h>  // define errno
#include <unistd.h> // sleep(1)
#include <iostream> // ? std::string
#include <fstream>

#include "USTPFtdcTraderApi.h"
#include "USTPFtdcMduserApi.h"
#include <pthread.h>

#include <dnj_femas_common.hpp>
#include <dnj_femas_api_input.hpp>
#include <dnj_femas_spi_output.hpp>
#include <dnj_femas_console.hpp>

using namespace std;

dnj_femas_global_setting dnj;

// cqq000
int main(int argc, char *argv[]) 
{

    // set up process dnj_famas start time
    dnj.ttJstart = get_currentTT();
    //printf("%s\n", dnj.ttJstart.timestamp);
    //printf("%s\n", dnj.ttJstart.timestamp_micro);
    //printf("%s\n", dnj.ttJstart.date);
    //printf("%s\n", dnj.ttJstart.time);
    //printf("%s\n", dnj.ttJstart.datentime);
    //printf("%s\n", dnj.ttJstart.datentime_micro);
    //printf("%s\n", dnj.ttJstart.nRequestID);
    //printf("%d\n", dnj.ttJstart.tv.tv_sec);
    //printf("%d\n", dnj.ttJstart.tv.tv_usec);


    // 那個顯示時間, 只是為了 keep connection
    pthread_t t_echo; // 宣告 pthread 變數
    pthread_create(&t_echo, NULL, echo_time, NULL); // 建立子執行緒
    //pthread_join(t_echo, NULL); // 等待子執行緒執行完成

    char sSource_au[10] = "au";
    char sCondition_au[10] = "if001";
    // load ./dnj_femas.cfg
    if (dnj.load_cfg_init(sSource_au, sCondition_au) == -1) {
        printf("No ./dnj_femas.cfg!!!\n");
        return 0;
    }
    /*
    printf("%s\n", dnj.basenamee   );
    printf("%s\n", dnj.gBrokerID   );
    printf("%s\n", dnj.gUserID     );
    printf("%s\n", dnj.gInvestorID );
    printf("%s\n", dnj.gPassword   );
    printf("%s\n", dnj.gExchangeID );
    printf("%s\n", dnj.gClientID   );
    printf("%s\n", dnj.gMAddr      );
    printf("%s\n", dnj.gTAddr      );
    */
        
/// T

    // 产生一个CUstpFtdcTraderApi实例
    CUstpFtdcTraderApi* pTUserApi = CUstpFtdcTraderApi::CreateFtdcTraderApi("./condatat/"); //j
    // 产生一个事件处理的实例
    TspiSimpleHandler tsh(pTUserApi); //相当于将 tsh 依附于 pTUserApi
    // 注册一事件处理的实例
    pTUserApi->RegisterSpi(&tsh); //pTUserApi 发生的一切事件都会调用 tsh 中的处理函数

    dnj.pTUserApi = pTUserApi;
    
    
    // 设置飞马平台服务的地址，可以注册多个地址备用
    pTUserApi -> RegisterFront(dnj.gTAddr);
    pTUserApi -> RegisterQryFront(dnj.gTAddr);
    
    // 订阅
    ///@remark 该方法要在Init方法前调用。若不调用则不会收到流的数据。
    // USTP_TERT_RESTART:从本交易日开始重传
    // USTP_TERT_RESUME:从上次收到的续传
    // USTP_TERT_QUICK:只传送登录后的内容
    // 订阅私有流
    pTUserApi->SubscribePrivateTopic(USTP_TERT_QUICK);
    // 订阅公共流
    pTUserApi->SubscribePublicTopic(USTP_TERT_QUICK);
    ///订阅交易员流。
    pTUserApi->SubscribeUserTopic(USTP_TERT_QUICK);
    ///订阅询价流。
    pTUserApi->SubscribeForQuote(USTP_TERT_QUICK);
            
    // 使客户端开始与后台服务建立连接
    dnj.ttTstart = get_currentTT();
    pTUserApi->Init();
 
//// M
    
    // 产生一个CUstpFtdcMduserApi实例
    CUstpFtdcMduserApi *pMUserApi = CUstpFtdcMduserApi::CreateFtdcMduserApi("./condatam/"); //j
    // 产生一个事件处理的实例
    MspiSimpleHandler msh(pMUserApi); //相当于将 msh 依附于 pMUserApi
    // 注册一事件处理的实例
    pMUserApi->RegisterSpi(&msh); //pMUserApi 发生的一切事件都会调用 msh 中的处理函数

    dnj.pMUserApi = pMUserApi;

    // 设置行情发布服务器的地址
    pMUserApi->RegisterFront(dnj.gMAddr);


    ///订阅市场行情。
    ///@param nTopicID 市场行情主题  
    ///@param nResumeType 市场行情重传方式  
    ///        USTP_TERT_RESTART:从本交易日开始重传
    ///        USTP_TERT_RESUME:从上次收到的续传(非订阅全部合约时，不支持续传模式)
    ///        USTP_TERT_QUICK:先传送当前行情快照,再传送登录后市场行情的内容
    ///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
    pMUserApi -> SubscribeMarketDataTopic (100, USTP_TERT_QUICK); //j 
//  pMUserApi -> SubscribeMarketDataTopic(500, USTP_TERT_RESTART);
//  pMUserApi -> SubscribeMarketDataTopic(301, USTP_TERT_RESTART);

    // 使客户端开始与行情发布服务器建立连接
    dnj.ttMstart = get_currentTT();
    pMUserApi->Init();


    // start read in.txt
    pthread_t tt_read_in_txt; // 宣告 pthread 變數
    pthread_create(&tt_read_in_txt, NULL, read_in_txt, NULL); // 建立子執行緒
    dnj.t_read_in_txt = &tt_read_in_txt;
    
    // start console 
    // r_source: 指令的來源:co (onsole 手動下)
    pthread_t t_read_console_in; // 宣告 pthread 變數
    pthread_create(&t_read_console_in, NULL, read_console_in, NULL); // 建立子執行緒
    //pthread_join(t_echo, NULL); // 等待子執行緒執行完成
    
    
    // main 主程式留給 au
    // r_source: 指令的來源:au (dnj_femas.out 自動)
    while (1) {
        sleep(60);
        ttTime tt_now = get_currentTT();
        // everyday 00:00
        // reload CFG, restart t_read_in_txt
        if (strcmp(tt_now.date, dnj.ttloadCFG.date) != 0) {
            // log?
            pthread_cancel(*dnj.t_read_in_txt);
            // reload CFG
            strcpy(sSource_au, "au");
            strcpy(sCondition_au, "if002");
            dnj.load_cfg_init(sSource_au, sCondition_au);
            // log?
            //    pthread_create(dnj.t_read_in_txt, NULL, read_in_txt, NULL); // 建立子執行緒 (這樣會 crash)
            pthread_create(&tt_read_in_txt, NULL, read_in_txt, NULL); // 建立子執行緒
            dnj.t_read_in_txt = &tt_read_in_txt;
        }
        // do T login
        if ((dnj.isTConnectOK == true) &&((dnj.isTLoginOK == false)) && dnj.isTauLogin){
            strcpy(sSource_au, "au");
            strcpy(sCondition_au, "if003");
            console_in_tqq01(sSource_au, sCondition_au);
        }
        // do M login
        if ((dnj.isMConnectOK == true) &&((dnj.isMLoginOK == false)) && dnj.isMauLogin){
            strcpy(sSource_au, "au");
            strcpy(sCondition_au, "if004");
            console_in_mqq01(sSource_au, sCondition_au);
        }
    }


    ///等待接口线程结束运行
    ///@return 线程退出代码
    pTUserApi->Join();
    pMUserApi->Join();

    // debug
    while(1) {
        printf("2 go there\n");
        sleep(1);
    }
    
    // 释放API实例
    pTUserApi->Release();
    pMUserApi->Release();
    return 0;
}