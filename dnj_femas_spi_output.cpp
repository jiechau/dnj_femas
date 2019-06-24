/*

改成用 cFields


P.S.
commit: 4d2eec2
最後一次 commit 用 
mrrXX append_file(dnj.filenamee_rr_xxx, sline);
trrXX append_file(dnj.filenamee_rr_xxx, sline);
trr40 append_file_persistent(dnj.filenamee_rr_rsp, sline);

*/
#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // strcmp
#include <float.h> // define DBL_MAX

#include <dnj_femas_common.hpp>
#include <dnj_femas_spi_output.hpp>
#include <dnj_femas_console.hpp>

#include "USTPFtdcUserApiStruct.h"

extern dnj_femas_global_setting dnj;

// class TspiSimpleHandler : public CUstpFtdcTraderSpi {
// public:
//     // 构造函数，需要一个有效的指向 CUstpFtdcMduserApi 实例的指针
//     TspiSimpleHandler(CUstpFtdcTraderApi* pUserApi) : t_pUserApi(pUserApi) {}
//     ~TspiSimpleHandler() {}
    
    
    // trr01 OnFrontConnected() 当客户端与飞马平台建立起通信连接，客户端需要进行登录
     void TspiSimpleHandler::OnFrontConnected()
    {
        dnj.ttTconnect = get_currentTT();
        dnj.isTConnectOK = true;
        ttTime tt_data = dnj.ttTconnect;
        printf("%s,trr01,OnFrontConnected()\n", dnj.ttTconnect.datentime_micro); 

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr01"                 );   
        sprintf(cFields[ 3], "%s", "OnFrontConnected()"    );
        sprintf(cFields[ 4], "%s", "trtn"                  );            

        send_sline_spi_output(cFields, 20);

        // 登录
        if (dnj.isTGoLogin) {
            char sSource_au[10] = "au";
            char sCondition_au[10] = "if005";
            console_in_tqq01(sSource_au, sCondition_au);
        }
    }
    
    ///trr02 
    void TspiSimpleHandler::OnQryFrontConnected()
    {
        printf(">>>>>>>>>>>>>>>>>>>>>,trr02,OnQryFrontConnected()\n");
    }
    
    // trr03 OnFrontDisconnected 当客户端与飞马平台通信连接断开时，该方法被调用
    void TspiSimpleHandler::OnFrontDisconnected(int nReason)
    {
        // 当发生这个情况后，API会自动重新连接，客户端可不做处理
        dnj.ttTdisconnect = get_currentTT();
        dnj.ttTlogout = get_currentTT();
        dnj.isTConnectOK = false;
        dnj.isTLoginOK = false; //??? 不確定這裡要不要 log rr_rsp (不確定會不會自動呼叫 logout())
        ttTime tt_data = dnj.ttTdisconnect;
        printf("%s,trr03,OnFrontDisconnected(),nReason=%d\n", dnj.ttTdisconnect.datentime_micro, nReason); 

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr03"                 );   
        sprintf(cFields[ 3], "%s", "OnFrontDisconnected()" );
        sprintf(cFields[ 4], "%s", "trtn"                  );            
      
        sprintf(cFields[20],"%d", nReason);   // nReason
        
        send_sline_spi_output(cFields, 21);
    }
    
    ///trr04
    void TspiSimpleHandler::OnQryFrontDisconnected(int nReason)
    {
        printf(">>>>>>>>>>>>>>>>>>>>,trr04,OnQryFrontDisconnected(),nReason=[%d]\n", nReason);
    }
    
    ///trr05
    void TspiSimpleHandler::OnHeartBeatWarning(int nTimeLapse){
        printf(">>>>>>>>>>>>>>>>>>>>,trr05,OnHeartBeatWarning(),nTimeLapse=[%d]\n", nTimeLapse);
    }

    ///trr06
    void TspiSimpleHandler::OnPackageStart(int nTopicID, int nSequenceNo){
//        printf("trr06:OnPackageStart(),nTopicID=[%d],nSequenceNo=[%d]\n", nTopicID, nSequenceNo);
    }
    
    ///trr07
    void TspiSimpleHandler::OnPackageEnd(int nTopicID, int nSequenceNo){
//        printf("trr07: OnPackageEnd(),nTopicID=[%d],nSequenceNo=[%d]\n", nTopicID, nSequenceNo);
    }
    
    // trr08 OnRspError() 针对用户请求的出错通知
    void TspiSimpleHandler::OnRspError(CUstpFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr08,OnRspError(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr08"                 );   
        sprintf(cFields[ 3], "%s", "OnRspError()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

        send_sline_spi_output(cFields, 20);
        // 客户端需进行错误处理
        // ErrorCode=[1],ErrorMsg=[Unrecognized ftd tid] (from 查询系统时间 ReqQrySystemTime())
        // ErrorCode=[1],ErrorMsg=[Not Login]
    }
    
    // trr09 OnRspUserLogin 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    void TspiSimpleHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField* pRspUserLogin, CUstpFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr09,OnRspUserLogin(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr09"                 );   
        sprintf(cFields[ 3], "%s", "OnRspUserLogin()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

        char uTradingSystemName[200];
        memset(&uTradingSystemName, 0, sizeof(uTradingSystemName));
        my_iconv("GB2312", "UTF-8", pRspUserLogin->TradingSystemName, uTradingSystemName); // (from, to, in, out)
        
        sprintf(cFields[20],"%s", pRspUserLogin->TradingDay       ) ; ///交易日                  TUstpFtdcDateType
        sprintf(cFields[21],"%s", pRspUserLogin->BrokerID         ) ; ///经纪公司编号            TUstpFtdcBrokerIDType
        sprintf(cFields[22],"%s", pRspUserLogin->UserID           ) ; ///交易用户代码            TUstpFtdcUserIDType    
        sprintf(cFields[23],"%s", pRspUserLogin->LoginTime        ) ; ///登录成功时间            TUstpFtdcTimeType    
        sprintf(cFields[24],"%s", pRspUserLogin->ExchangeTime     ) ; ///登录成功时的交易所时间  TUstpFtdcTimeType    
        sprintf(cFields[25],"%s", pRspUserLogin->MaxOrderLocalID  ) ; ///用户最大本地报单号      TUstpFtdcUserOrderLocalIDType    
        sprintf(cFields[26],"%s", uTradingSystemName              ) ; ///交易系统名称            TUstpFtdcTradingSystemNameType    
        sprintf(cFields[27],"%d", pRspUserLogin->DataCenterID     ) ; ///数据中心代码            TUstpFtdcDataCenterIDType    
        sprintf(cFields[28],"%d", pRspUserLogin->PrivateFlowSize  ) ; ///会员私有流当前长度      TUstpFtdcSequenceNoType    
        sprintf(cFields[29],"%d", pRspUserLogin->UserFlowSize     ) ; ///交易员私有流当前长度    TUstpFtdcSequenceNoType   
        sprintf(cFields[30],"%s", pRspUserLogin->ActionDay        ) ; ///业务发生日期            TUstpFtdcDateType    
        sprintf(cFields[31],"%s", pRspUserLogin->FemasVersion     ) ; ///飞马版本号              TUstpFtdcFemasVersionType    
        sprintf(cFields[32],"%d", pRspUserLogin->FemasLifeCycle   ) ; ///飞马生命周期号          TUstpFtdcFemasLifeCycleType    

        send_sline_spi_output(cFields, 33);
        
        if (pRspInfo->ErrorID == 0) {
            // 登录成功
            dnj.ttTlogin = tt_data;
            dnj.isTLoginOK = true;
        } else {
            // 指令失败，客户端需进行错误处理 (不一定會登出, 只是不改變狀態)
            // ErrorCode=[106],ErrorMsg=[重复的会话]
        }
        
    }
    // end of void TspiSimpleHandler::OnRspUserLogin
    
    
    // trr10 OnRspUserLogout 用户退出应答
    void TspiSimpleHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr10,OnRspUserLogout(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr10"                 );   
        sprintf(cFields[ 3], "%s", "OnRspUserLogout()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 
        
        sprintf(cFields[20],"%s", pRspUserLogout->BrokerID         ) ; ///经纪公司编号 TUstpFtdcBrokerIDType
        sprintf(cFields[21],"%s", pRspUserLogout->UserID           ) ; ///交易用户代码 TUstpFtdcUserIDType    

        send_sline_spi_output(cFields, 22);
        
        if (pRspInfo->ErrorID == 0) {
            // 退出成功
            dnj.ttTlogout  = tt_data;
            dnj.isTLoginOK = false;
        } else {
            // 指令失败，客户端需进行错误处理 (不一定會登出, 只是不改變狀態)
            // ErrorCode=
        }
    }
    
    ///trr11 用户密码修改应答
    void TspiSimpleHandler::OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr11:OnRspUserPasswordUpdate():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    // trr12 OnRspOrderInsert 报单录入应答
    void TspiSimpleHandler::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr12,OnRspOrderInsert(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        printf("                          ,trr12:OrderSysID=[%s]\n", pInputOrder->OrderSysID);
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr12"                 );   
        sprintf(cFields[ 3], "%s", "OnRspOrderInsert()"    );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
        //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

                                                                                    sprintf(cFields[20],"%s", pInputOrder->BrokerID         ); ///经纪公司编号            TUstpFtdcBrokerIDType               
                                                                                    sprintf(cFields[21],"%s", pInputOrder->ExchangeID       ); ///交易所代码              TUstpFtdcExchangeIDType               
                                                                                    sprintf(cFields[22],"%s", pInputOrder->OrderSysID       ); ///系统报单编号            TUstpFtdcOrderSysIDType             
                                                                                    sprintf(cFields[23],"%s", pInputOrder->InvestorID       ); ///投资者编号              TUstpFtdcInvestorIDType               
                                                                                    sprintf(cFields[24],"%s", pInputOrder->UserID           ); ///用户代码                TUstpFtdcUserIDType                     
                                                                                    sprintf(cFields[25],"%d", pInputOrder->SeatNo           ); ///指定通过此席位序号下单  TUstpFtdcSeatNoType       
                                                                                    sprintf(cFields[26],"%s", pInputOrder->InstrumentID     ); ///合约代码/套利组合合约号 TUstpFtdcInstrumentIDType
                                                                                    sprintf(cFields[27],"%s", pInputOrder->UserOrderLocalID ); ///用户本地报单号          TUstpFtdcUserOrderLocalIDType     
                                                                                    sprintf(cFields[28],"%c", pInputOrder->OrderPriceType   ); ///报单类型                TUstpFtdcOrderPriceTypeType             
                                                                                    sprintf(cFields[29],"%c", pInputOrder->Direction        ); ///买卖方向                TUstpFtdcDirectionType                  
                                                                                    sprintf(cFields[30],"%c", pInputOrder->OffsetFlag       ); ///开平标志                TUstpFtdcOffsetFlagType                 
                                                                                    sprintf(cFields[31],"%c", pInputOrder->HedgeFlag        ); ///投机套保标志            TUstpFtdcHedgeFlagType              
        (pInputOrder->LimitPrice         == DBL_MAX) ? sprintf(cFields[32],"-"):    sprintf(cFields[32],"%f", pInputOrder->LimitPrice       ); ///价格                    TUstpFtdcPriceType                          
                                                                                    sprintf(cFields[33],"%d", pInputOrder->Volume           ); ///数量                    TUstpFtdcVolumeType                         
                                                                                    sprintf(cFields[34],"%c", pInputOrder->TimeCondition    ); ///有效期类型              TUstpFtdcTimeConditionType            
                                                                                    sprintf(cFields[35],"%s", pInputOrder->GTDDate          ); ///GTD日期                 TUstpFtdcDateType                        
                                                                                    sprintf(cFields[36],"%c", pInputOrder->VolumeCondition  ); ///成交量类型              TUstpFtdcVolumeConditionType          
                                                                                    sprintf(cFields[37],"%d", pInputOrder->MinVolume        ); ///最小成交量              TUstpFtdcVolumeType                   
        (pInputOrder->StopPrice          == DBL_MAX) ? sprintf(cFields[38],"-")   : sprintf(cFields[38],"%f", pInputOrder->StopPrice        ); ///止损价止赢价            TUstpFtdcPriceType                  
                                                                                    sprintf(cFields[39],"%c", pInputOrder->ForceCloseReason ); ///强平原因                TUstpFtdcForceCloseReasonType           
                                                                                    sprintf(cFields[40],"%d", pInputOrder->IsAutoSuspend    ); ///自动挂起标志            TUstpFtdcBoolType                   
                                                                                    sprintf(cFields[41],"%s", pInputOrder->BusinessUnit     ); ///业务单元                TUstpFtdcBusinessUnitType               
                                                                                    sprintf(cFields[42],"%s", pInputOrder->UserCustom       ); ///用户自定义域            TUstpFtdcCustomType                 
                                                                                    sprintf(cFields[43],"%d", pInputOrder->BusinessLocalID  ); ///本地业务标识            TUstpFtdcBusinessLocalIDType        
                                                                                    sprintf(cFields[44],"%s", pInputOrder->ActionDay        ); ///业务发生日期            TUstpFtdcDateType                   
                                                                                    sprintf(cFields[45],"%c", pInputOrder->ArbiType         ); ///策略类别                TUstpFtdcArbiTypeType                   
                                                                                    sprintf(cFields[46],"%s", pInputOrder->ClientID         ); ///客户编码                TUstpFtdcClientIDType    
        send_sline_spi_output(cFields, 47);
        /*
        ErrorCode=[12], ErrorMsg=[重复的报单]
        ErrorCode=[26], ErrorMsg=[交易所返回:[当前状态禁止此项操作]]
        ErrorCode=[36], ErrorMsg=[资金不足]
        ErrorCode=[37], ErrorMsg=[不合法的数量] (option 用市價/任意價. 因為只能用限價)
        ErrorCode=[49], ErrorMsg=[交易所返回:[价格超出涨停板]] (option)
        ErrorCode=[79], ErrorMsg=[交易所返回:[不被支持的报单类型]] 
        ErrorCode=[125], ErrorMsg=[交易所返回:[超出当时价格保护带上带价格:3873.600000]] (future)
        ErrorCode=[138], ErrorMsg=[交易所返回:[订单触发自成交]] (上一批 order 還沒消化完, 就開始平)
        ErrorCode=[8010], ErrorMsg=[报单字段检查ORDERPRICETYPE错误]
        ErrorCode=[8016], ErrorMsg=[报单字段检查VOLUMECONDITION错误]
        ErrorCode=[8018], ErrorMsg=[报单字段检查FORCECLOSEREASON错误]
        ErrorCode=[8136], ErrorMsg=[找不到客户费率信息]
        ErrorCode=[10005],ErrorMsg=[市场已收盘]
        */
    }
    
    // trr13 OnRspOrderAction() 报单操作应答
    void TspiSimpleHandler::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr13,OnRspOrderAction(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        printf("                          ,trr13:OrderSysID=[%s]\n", pOrderAction->OrderSysID);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr13"                 );   
        sprintf(cFields[ 3], "%s", "OnRspOrderAction()"    );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
        //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );    
        //send_sline_spi_output(cFields, 32);
                                                                                    sprintf(cFields[20],"%s", pOrderAction->ExchangeID             ); ///交易所代码             TUstpFtdcExchangeIDType                  
                                                                                    sprintf(cFields[21],"%s", pOrderAction->OrderSysID             ); ///系统报单编号           TUstpFtdcOrderSysIDType                
                                                                                    sprintf(cFields[22],"%s", pOrderAction->BrokerID               ); ///经纪公司编号           TUstpFtdcBrokerIDType                  
                                                                                    sprintf(cFields[23],"%s", pOrderAction->InvestorID             ); ///投资者编号             TUstpFtdcInvestorIDType                  
                                                                                    sprintf(cFields[24],"%s", pOrderAction->UserID                 ); ///用户代码               TUstpFtdcUserIDType                        
                                                                                    sprintf(cFields[25],"%s", pOrderAction->UserOrderActionLocalID ); ///本次撤单操作的本地编号 TUstpFtdcUserOrderLocalIDType
                                                                                    sprintf(cFields[26],"%s", pOrderAction->UserOrderLocalID       ); ///被撤订单的本地报单编号 TUstpFtdcUserOrderLocalIDType
                                                                                    sprintf(cFields[27],"%c", pOrderAction->ActionFlag             ); ///报单操作标志           TUstpFtdcActionFlagType （只支持删除）
        (pOrderAction->LimitPrice        == DBL_MAX) ? sprintf(cFields[18],"-") :   sprintf(cFields[28],"%f", pOrderAction->LimitPrice             ); ///价格                   TUstpFtdcPriceType （暂不支持，保留域）
                                                                                    sprintf(cFields[29],"%d", pOrderAction->VolumeChange           ); ///数量变化               TUstpFtdcVolumeType （暂不支持，保留域）
                                                                                    sprintf(cFields[30],"%d", pOrderAction->BusinessLocalID        ); ///本地业务标识           TUstpFtdcBusinessLocalIDType           
                                                                                    sprintf(cFields[31],"%s", pOrderAction->ClientID               ); ///客户编码               TUstpFtdcClientIDType                      
        send_sline_spi_output(cFields, 32);
    }

    ///报价录入应答
    void TspiSimpleHandler::OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr14:OnRspQuoteInsert():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///报价操作应答
    void TspiSimpleHandler::OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr15:OnRspQuoteAction():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///询价请求应答
    void TspiSimpleHandler::OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr16:OnRspForQuote():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///客户申请组合应答
    void TspiSimpleHandler::OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr17:OnRspMarginCombAction():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///用户请求出入金应答
    void TspiSimpleHandler::OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr18:OnRspUserDeposit():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///用户主次席出入金应答
    void TspiSimpleHandler::OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr19:OnRspTransferMoney():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    ///数据流回退通知
    void TspiSimpleHandler::OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {
        printf("trr20:OnRtnFlowMessageCancel():");
        printf("SequenceSeries=[%d],TradingDay=[%s],DataCenterID=[%d],start=[%d],end=[%d]\n", pFlowMessageCancel->SequenceSeries, pFlowMessageCancel->TradingDay, pFlowMessageCancel->DataCenterID, pFlowMessageCancel->StartSequenceNo, pFlowMessageCancel->EndSequenceNo);
    }

    // trr21 OnRtnTrade() 成交回报 // much info
    void TspiSimpleHandler::OnRtnTrade(CUstpFtdcTradeField *pTrade) {           
        ttTime tt_data = get_currentTT();
        printf("%s,trr21,OnRtnTrade(),www,OrderSysID=[%s],TradeID=[%s],TradeVolume=[%d]\n", tt_data.datentime_micro, pTrade->OrderSysID, pTrade->TradeID, pTrade->TradeVolume); //www
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr21"                 );   
        sprintf(cFields[ 3], "%s", "OnRtnTrade()"  );
        sprintf(cFields[ 4], "%s", "trtn"                  );            
        
                                                                        sprintf(cFields[20],"%s", pTrade->BrokerID         ); ///经纪公司编号 TUstpFtdcBrokerIDType          
                                                                        sprintf(cFields[21],"%s", pTrade->ExchangeID       ); ///交易所代码 TUstpFtdcExchangeIDType        
                                                                        sprintf(cFields[22],"%s", pTrade->TradingDay       ); ///交易日 TUstpFtdcTradingDayType            
                                                                        sprintf(cFields[23],"%s", pTrade->ParticipantID    ); ///会员编号 TUstpFtdcParticipantIDType       
                                                                        sprintf(cFields[24],"%s", pTrade->SeatID           ); ///下单席位号 TUstpFtdcSeatIDType            
                                                                        sprintf(cFields[25],"%s", pTrade->InvestorID       ); ///投资者编号 TUstpFtdcInvestorIDType        
                                                                        sprintf(cFields[26],"%s", pTrade->ClientID         ); ///客户编码 TUstpFtdcClientIDType            
                                                                        sprintf(cFields[27],"%s", pTrade->UserID           ); ///用户编号 TUstpFtdcUserIDType              
                                                                        sprintf(cFields[28],"%s", pTrade->OrderUserID      ); ///下单用户编号 TUstpFtdcUserIDType          
                                                                        sprintf(cFields[29],"%s", pTrade->TradeID          ); ///成交编号 TUstpFtdcTradeIDType             
                                                                        sprintf(cFields[30],"%s", pTrade->OrderSysID       ); ///系统报单编号 TUstpFtdcOrderSysIDType      
                                                                        sprintf(cFields[31],"%s", pTrade->UserOrderLocalID ); ///本地报单编号 TUstpFtdcUserOrderLocalIDType
                                                                        sprintf(cFields[32],"%s", pTrade->InstrumentID     ); ///合约代码 TUstpFtdcInstrumentIDType        
                                                                        sprintf(cFields[33],"%c", pTrade->Direction        ); ///买卖方向 TUstpFtdcDirectionType           
                                                                        sprintf(cFields[34],"%c", pTrade->OffsetFlag       ); ///开平标志 TUstpFtdcOffsetFlagType          
                                                                        sprintf(cFields[35],"%c", pTrade->HedgeFlag        ); ///投机套保标志 TUstpFtdcHedgeFlagType       
        (pTrade->TradePrice == DBL_MAX) ? sprintf(cFields[36],"-")    : sprintf(cFields[36],"%f", pTrade->TradePrice       ); ///成交价格 TUstpFtdcPriceType               
                                                                        sprintf(cFields[37],"%d", pTrade->TradeVolume      ); ///成交数量 TUstpFtdcVolumeType              
                                                                        sprintf(cFields[38],"%s", pTrade->TradeTime        ); ///成交时间 TUstpFtdcTimeType                
                                                                        sprintf(cFields[39],"%s", pTrade->ClearingPartID   ); ///清算会员编号 TUstpFtdcParticipantIDType   
        (pTrade->UsedFee    == DBL_MAX) ? sprintf(cFields[40],"-")    : sprintf(cFields[40],"%f", pTrade->UsedFee          ); ///本次成交手续费 TUstpFtdcMoneyType         
        (pTrade->UsedMargin == DBL_MAX) ? sprintf(cFields[41],"-")    : sprintf(cFields[41],"%f", pTrade->UsedMargin       ); ///本次成交占用保证金 TUstpFtdcMoneyType     
        (pTrade->Premium    == DBL_MAX) ? sprintf(cFields[42],"-")    : sprintf(cFields[42],"%f", pTrade->Premium          ); ///本次成交占用权利金 TUstpFtdcMoneyType     
                                                                        sprintf(cFields[43],"%d", pTrade->Position         ); ///持仓表今持仓量 TUstpFtdcVolumeType        
        (pTrade->PositionCost == DBL_MAX) ? sprintf(cFields[44],"-")  : sprintf(cFields[44],"%f", pTrade->PositionCost     ); ///持仓表今日持仓成本 TUstpFtdcPriceType     
        (pTrade->Available    == DBL_MAX) ? sprintf(cFields[45],"-")  : sprintf(cFields[45],"%f", pTrade->Available        ); ///资金表可用资金 TUstpFtdcMoneyType         
        (pTrade->Margin       == DBL_MAX) ? sprintf(cFields[46],"-")  : sprintf(cFields[46],"%f", pTrade->Margin           ); ///资金表占用保证金 TUstpFtdcMoneyType       
        (pTrade->FrozenMargin == DBL_MAX) ? sprintf(cFields[47],"-")  : sprintf(cFields[47],"%f", pTrade->FrozenMargin     ); ///资金表冻结的保证金 TUstpFtdcMoneyType     
                                                                        sprintf(cFields[48],"%d", pTrade->BusinessLocalID  ); ///本地业务标识 TUstpFtdcBusinessLocalIDType 
                                                                        sprintf(cFields[49],"%s", pTrade->ActionDay        ); ///业务发生日期 TUstpFtdcDateType            
                                                                        sprintf(cFields[50],"%c", pTrade->ArbiType         ); ///策略类别 TUstpFtdcArbiTypeType            
                                                                        sprintf(cFields[51],"%s", pTrade->ArbiInstrumentID ); ///套利组合合约 TUstpFtdcInstrumentIDType    
                                                                                        
        send_sline_spi_output(cFields, 52);                                             
    }                                                                                   
                                                                                        
    // trr22 OnRtnOrder() 报单回报 // much info                                         
    void TspiSimpleHandler::OnRtnOrder(CUstpFtdcOrderField* pOrder) {
        ttTime tt_data = get_currentTT();
        printf("%s,trr22,OnRtnOrder(),www,OrderSysID=[%s],Volume=[%d],VolumeRemain=[%d]\n", tt_data.datentime_micro, pOrder->OrderSysID, pOrder->Volume, pOrder->VolumeRemain); //www
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr22"                 );   
        sprintf(cFields[ 3], "%s", "OnRtnOrder()"  );
        sprintf(cFields[ 4], "%s", "trtn"                  );            
        
                                                                        sprintf(cFields[20],"%s", pOrder->BrokerID        );  ///经纪公司编号 TUstpFtdcBrokerIDType                    
                                                                        sprintf(cFields[21],"%s", pOrder->ExchangeID      );  ///交易所代码 TUstpFtdcExchangeIDType                    
                                                                        sprintf(cFields[22],"%s", pOrder->OrderSysID      );  ///系统报单编号 TUstpFtdcOrderSysIDType                    
                                                                        sprintf(cFields[23],"%s", pOrder->InvestorID      );  ///投资者编号 TUstpFtdcInvestorIDType                    
                                                                        sprintf(cFields[24],"%s", pOrder->UserID          );  ///用户代码 TUstpFtdcUserIDType                            
                                                                        sprintf(cFields[25],"%d", pOrder->SeatNo          );  ///指定通过此席位序号下单 TUstpFtdcSeatNoType            
                                                                        sprintf(cFields[26],"%s", pOrder->InstrumentID    );  ///合约代码/套利组合合约号 TUstpFtdcInstrumentIDType    
                                                                        sprintf(cFields[27],"%s", pOrder->UserOrderLocalID);  ///用户本地报单号 TUstpFtdcUserOrderLocalIDType            
                                                                        sprintf(cFields[28],"%c", pOrder->OrderPriceType   );  ///报单类型 TUstpFtdcOrderPriceTypeType                    
                                                                        sprintf(cFields[29],"%c", pOrder->Direction        );  ///买卖方向 TUstpFtdcDirectionType                        
                                                                        sprintf(cFields[30],"%c", pOrder->OffsetFlag       );  ///开平标志 TUstpFtdcOffsetFlagType                        
                                                                        sprintf(cFields[31],"%c", pOrder->HedgeFlag        );  ///投机套保标志 TUstpFtdcHedgeFlagType                    
        (pOrder->LimitPrice == DBL_MAX) ? sprintf(cFields[32],"-")    : sprintf(cFields[32],"%f", pOrder->LimitPrice       );  ///价格 TUstpFtdcPriceType                                
                                                                        sprintf(cFields[33],"%d", pOrder->Volume           );  ///数量 TUstpFtdcVolumeType                                
                                                                        sprintf(cFields[34],"%c", pOrder->TimeCondition    );  ///有效期类型 TUstpFtdcTimeConditionType                
                                                                        sprintf(cFields[35],"%s", pOrder->GTDDate          );  ///GTD日期 TUstpFtdcDateType                            
                                                                        sprintf(cFields[36],"%c", pOrder->VolumeCondition  );  ///成交量类型 TUstpFtdcVolumeConditionType                
                                                                        sprintf(cFields[37],"%d", pOrder->MinVolume        );  ///最小成交量 TUstpFtdcVolumeType                        
        (pOrder->StopPrice  == DBL_MAX) ? sprintf(cFields[38],"-")    : sprintf(cFields[38],"%f", pOrder->StopPrice        );  ///止损价止赢价 TUstpFtdcPriceType                        
                                                                        sprintf(cFields[39],"%c", pOrder->ForceCloseReason );  ///强平原因 TUstpFtdcForceCloseReasonType                
                                                                        sprintf(cFields[40],"%d", pOrder->IsAutoSuspend    );  ///自动挂起标志 TUstpFtdcBoolType                        
                                                                        sprintf(cFields[41],"%s", pOrder->BusinessUnit     );  ///业务单元 TUstpFtdcBusinessUnitType                    
                                                                        sprintf(cFields[42],"%s", pOrder->UserCustom       );  ///用户自定义域 TUstpFtdcCustomType                        
                                                                        sprintf(cFields[43],"%d", pOrder->BusinessLocalID  );  ///本地业务标识 TUstpFtdcBusinessLocalIDType            
                                                                        sprintf(cFields[44],"%s", pOrder->ActionDay        );  ///业务发生日期 TUstpFtdcDateType                        
                                                                        sprintf(cFields[45],"%c", pOrder->ArbiType         );  ///策略类别 TUstpFtdcArbiTypeType                        
                                                                        sprintf(cFields[46],"%s", pOrder->TradingDay       );  ///交易日 TUstpFtdcTradingDayType                        
                                                                        sprintf(cFields[47],"%s", pOrder->ParticipantID    );  ///会员编号 TUstpFtdcParticipantIDType                    
                                                                        sprintf(cFields[48],"%s", pOrder->OrderUserID      );  ///最初下单用户编号 TUstpFtdcUserIDType                    
                                                                        sprintf(cFields[49],"%s", pOrder->ClientID         );  ///客户编码 TUstpFtdcClientIDType                        
                                                                        sprintf(cFields[50],"%s", pOrder->SeatID           );  ///下单席位号 TUstpFtdcSeatIDType                        
                                                                        sprintf(cFields[51],"%s", pOrder->InsertTime       );  ///插入时间 TUstpFtdcTimeType                            
                                                                        sprintf(cFields[52],"%s", pOrder->OrderLocalID     );  ///本地报单编号 TUstpFtdcOrderLocalIDType                
                                                                        sprintf(cFields[53],"%c", pOrder->OrderSource      );  ///报单来源 TUstpFtdcOrderSourceType                    
                                                                        sprintf(cFields[54],"%c", pOrder->OrderStatus      );  ///报单状态 TUstpFtdcOrderStatusType                    
                                                                        sprintf(cFields[55],"%s", pOrder->CancelTime       );  ///撤销时间 TUstpFtdcTimeType                            
                                                                        sprintf(cFields[56],"%s", pOrder->CancelUserID     );  ///撤单用户编号 TUstpFtdcUserIDType                        
                                                                        sprintf(cFields[57],"%d", pOrder->VolumeTraded     );  ///今成交数量 TUstpFtdcVolumeType                        
                                                                        sprintf(cFields[58],"%d", pOrder->VolumeRemain     );  ///剩余数量 TUstpFtdcVolumeType                            
                                                                        sprintf(cFields[59],"%c", pOrder->OrderType        );  ///委托类型 TUstpFtdcOrderTypeType                        
                                                                        sprintf(cFields[60],"%c", pOrder->DeliveryFlag     );  ///期权对冲标识 TUstpFtdcDeliveryFlagType                
        send_sline_spi_output(cFields, 61); 
           
    }

    // trr23 OnErrRtnOrderInsert() 报单录入错误回报
    void TspiSimpleHandler::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr23,OnErrRtnOrderInsert(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],", pRspInfo->ErrorID, uErrorMsg);
        printf("OrderSysID=[%s]\n", pInputOrder->OrderSysID);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr23"                 );   
        sprintf(cFields[ 3], "%s", "OnErrRtnOrderInsert()"  );
        sprintf(cFields[ 4], "%s", "terr"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );


        //
        //
        
        

                                                                                 
                                                                                    
                                                                                    sprintf(cFields[20],"%s", pInputOrder->BrokerID         ); ///经纪公司编号            TUstpFtdcBrokerIDType
                                                                                    sprintf(cFields[21],"%s", pInputOrder->ExchangeID       ); ///交易所代码              TUstpFtdcExchangeIDType
                                                                                    sprintf(cFields[22],"%s", pInputOrder->OrderSysID       ); ///系统报单编号            TUstpFtdcOrderSysIDType
                                                                                    sprintf(cFields[23],"%s", pInputOrder->InvestorID       ); ///投资者编号              TUstpFtdcInvestorIDType
                                                                                    sprintf(cFields[24],"%s", pInputOrder->UserID           ); ///用户代码                TUstpFtdcUserIDType  
                                                                                    sprintf(cFields[25],"%d", pInputOrder->SeatNo           ); ///指定通过此席位序号下单  TUstpFtdcSeatNoType
                                                                                    sprintf(cFields[26],"%s", pInputOrder->InstrumentID     ); ///合约代码/套利组合合约号 TUstpFtdcInstrumentIDType
                                                                                    sprintf(cFields[27],"%s", pInputOrder->UserOrderLocalID ); ///用户本地报单号          TUstpFtdcUserOrderLocalIDType    
                                                                                    sprintf(cFields[28],"%c", pInputOrder->OrderPriceType   ); ///报单类型                TUstpFtdcOrderPriceTypeType        
                                                                                    sprintf(cFields[29],"%c", pInputOrder->Direction        ); ///买卖方向                TUstpFtdcDirectionType        
                                                                                    sprintf(cFields[30],"%c", pInputOrder->OffsetFlag       ); ///开平标志                TUstpFtdcOffsetFlagType        
                                                                                    sprintf(cFields[31],"%c", pInputOrder->HedgeFlag        ); ///投机套保标志            TUstpFtdcHedgeFlagType
        (pInputOrder->LimitPrice         == DBL_MAX) ? sprintf(cFields[32],"-")   : sprintf(cFields[32],"%f", pInputOrder->LimitPrice       ); ///价格                    TUstpFtdcPriceType     
                                                                                    sprintf(cFields[33],"%d", pInputOrder->Volume           ); ///数量                    TUstpFtdcVolumeType     
                                                                                    sprintf(cFields[34],"%c", pInputOrder->TimeCondition    ); ///有效期类型              TUstpFtdcTimeConditionType    
                                                                                    sprintf(cFields[35],"%s", pInputOrder->GTDDate          ); ///GTD日期                 TUstpFtdcDateType  
                                                                                    sprintf(cFields[36],"%c", pInputOrder->VolumeCondition  ); ///成交量类型              TUstpFtdcVolumeConditionType    
                                                                                    sprintf(cFields[37],"%d", pInputOrder->MinVolume        ); ///最小成交量              TUstpFtdcVolumeType    
        (pInputOrder->StopPrice          == DBL_MAX) ? sprintf(cFields[38],"-")   : sprintf(cFields[38],"%f", pInputOrder->StopPrice        ); ///止损价止赢价            TUstpFtdcPriceType
                                                                                    sprintf(cFields[39],"%c", pInputOrder->ForceCloseReason ); ///强平原因                TUstpFtdcForceCloseReasonType     
                                                                                    sprintf(cFields[40],"%d", pInputOrder->IsAutoSuspend    ); ///自动挂起标志            TUstpFtdcBoolType
                                                                                    sprintf(cFields[41],"%s", pInputOrder->BusinessUnit     ); ///业务单元                TUstpFtdcBusinessUnitType     
                                                                                    sprintf(cFields[42],"%s", pInputOrder->UserCustom       ); ///用户自定义域            TUstpFtdcCustomType
                                                                                    sprintf(cFields[43],"%d", pInputOrder->BusinessLocalID  ); ///本地业务标识            TUstpFtdcBusinessLocalIDType
                                                                                    sprintf(cFields[44],"%s", pInputOrder->ActionDay        ); ///业务发生日期            TUstpFtdcDateType
                                                                                    sprintf(cFields[45],"%c", pInputOrder->ArbiType         ); ///策略类别                TUstpFtdcArbiTypeType     
                                                                                    sprintf(cFields[46],"%s", pInputOrder->ClientID         ); ///客户编码                TUstpFtdcClientIDType
                                                                                                    
        send_sline_spi_output(cFields, 47);                                                         
        // ErrorCode=[31],ErrorMsg=[交易所返回:[平仓时客户持仓不足]],OrderSysID=[]                  

    }

    // trr24 OnErrRtnOrderAction() 报单操作错误回报
    void TspiSimpleHandler::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr24,OnErrRtnOrderAction(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],", pRspInfo->ErrorID, uErrorMsg);
        printf("OrderSysID=[%s]\n", pOrderAction->OrderSysID);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr24"                 );   
        sprintf(cFields[ 3], "%s", "OnErrRtnOrderAction()"  );
        sprintf(cFields[ 4], "%s", "terr"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        

                                                                                    sprintf(cFields[20],"%s", pOrderAction->ExchangeID             ); ///交易所代码             TUstpFtdcExchangeIDType                  
                                                                                    sprintf(cFields[21],"%s", pOrderAction->OrderSysID             ); ///系统报单编号           TUstpFtdcOrderSysIDType                
                                                                                    sprintf(cFields[22],"%s", pOrderAction->BrokerID               ); ///经纪公司编号           TUstpFtdcBrokerIDType                  
                                                                                    sprintf(cFields[23],"%s", pOrderAction->InvestorID             ); ///投资者编号             TUstpFtdcInvestorIDType                  
                                                                                    sprintf(cFields[24],"%s", pOrderAction->UserID                 ); ///用户代码               TUstpFtdcUserIDType                        
                                                                                    sprintf(cFields[25],"%s", pOrderAction->UserOrderActionLocalID ); ///本次撤单操作的本地编号 TUstpFtdcUserOrderLocalIDType
                                                                                    sprintf(cFields[26],"%s", pOrderAction->UserOrderLocalID       ); ///被撤订单的本地报单编号 TUstpFtdcUserOrderLocalIDType
                                                                                    sprintf(cFields[27],"%c", pOrderAction->ActionFlag             ); ///报单操作标志           TUstpFtdcActionFlagType                
        (pOrderAction->LimitPrice           == DBL_MAX) ? sprintf(cFields[28],"-"): sprintf(cFields[28],"%f", pOrderAction->LimitPrice             ); ///价格                   TUstpFtdcPriceType                             
                                                                                    sprintf(cFields[29],"%d", pOrderAction->VolumeChange           ); ///数量变化               TUstpFtdcVolumeType                        
                                                                                    sprintf(cFields[30],"%d", pOrderAction->BusinessLocalID        ); ///本地业务标识           TUstpFtdcBusinessLocalIDType           
                                                                                    sprintf(cFields[31],"%s", pOrderAction->ClientID               ); ///客户编码               TUstpFtdcClientIDType                      
                                                                                                    
        send_sline_spi_output(cFields, 32);                                                         
        // ErrorCode=[24],ErrorMsg=[报单找不到]                                                     
    }

    // trr25 OnRtnInstrumentStatus()  合约交易状态通知
    void TspiSimpleHandler::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) {
        ttTime tt_data = get_currentTT();
        //printf("%s,trr25,OnRtnInstrumentStatus(),www,InstrumentID=[%s]\n", tt_data.datentime_micro, pInstrumentStatus->InstrumentID); //www

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr25"                 );   
        sprintf(cFields[ 3], "%s", "OnRtnInstrumentStatus()"  );
        sprintf(cFields[ 4], "%s", "trtn"                  );            
        
        char uProductName[200];
        memset(&uProductName, 0, sizeof(uProductName));
        my_iconv("GB2312", "UTF-8", pInstrumentStatus->ProductName, uProductName); // (from, to, in, out)
        char uInstrumentName[200];
        memset(&uInstrumentName, 0, sizeof(uInstrumentName));
        my_iconv("GB2312", "UTF-8", pInstrumentStatus->InstrumentName, uInstrumentName); // (from, to, in, out)
                                                                      
                                                                                        sprintf(cFields[20],"%s", pInstrumentStatus->ExchangeID           );  ///交易所代码 TUstpFtdcExchangeIDType           CFFEX 
                                                                                        sprintf(cFields[21],"%s", pInstrumentStatus->ProductID            );  ///品种代码 TUstpFtdcProductIDType               
                                                                                        sprintf(cFields[22],"%s", uProductName                            );  ///品种名称 TUstpFtdcProductNameType             
                                                                                        sprintf(cFields[23],"%s", pInstrumentStatus->InstrumentID         );  ///合约代码 TUstpFtdcInstrumentIDType           IO1912-P-3100  
                                                                                        sprintf(cFields[24],"%s", uInstrumentName                         );  ///合约名称 TUstpFtdcInstrumentNameType             
                                                                                        sprintf(cFields[25],"%d", pInstrumentStatus->DeliveryYear         );  ///交割年份 TUstpFtdcYearType                   1900  
                                                                                        sprintf(cFields[26],"%d", pInstrumentStatus->DeliveryMonth        );  ///交割月 TUstpFtdcMonthType                    1 
                                                                                        sprintf(cFields[27],"%d", pInstrumentStatus->MaxLimitOrderVolume  );  ///限价单最大下单量 TUstpFtdcVolumeType         0    
                                                                                        sprintf(cFields[28],"%d", pInstrumentStatus->MinLimitOrderVolume  );  ///限价单最小下单量 TUstpFtdcVolumeType         0    
                                                                                        sprintf(cFields[29],"%d", pInstrumentStatus->MaxMarketOrderVolume );  ///市价单最大下单量 TUstpFtdcVolumeType         0    
                                                                                        sprintf(cFields[30],"%d", pInstrumentStatus->MinMarketOrderVolume );  ///市价单最小下单量 TUstpFtdcVolumeType         0    
                                                                                        sprintf(cFields[31],"%d", pInstrumentStatus->VolumeMultiple       );  ///数量乘数 TUstpFtdcVolumeMultipleType         (null)    
        (pInstrumentStatus->PriceTick       == DBL_MAX) ? sprintf(cFields[32],"-")    : sprintf(cFields[32],"%f", pInstrumentStatus->PriceTick            );  ///报价单位 TUstpFtdcPriceTickType              0.000000   
                                                                                        sprintf(cFields[33],"%c", pInstrumentStatus->Currency             );  ///币种 TUstpFtdcCurrencyType                   ???
                                                                                        sprintf(cFields[34],"%d", pInstrumentStatus->LongPosLimit         );  ///多头限仓 TUstpFtdcVolumeType                 0  
                                                                                        sprintf(cFields[35],"%d", pInstrumentStatus->ShortPosLimit        );  ///空头限仓 TUstpFtdcVolumeType                 0      
        (pInstrumentStatus->LowerLimitPrice == DBL_MAX) ? sprintf(cFields[36],"-")    : sprintf(cFields[36],"%f", pInstrumentStatus->LowerLimitPrice      );  ///跌停板价 TUstpFtdcPriceType                  0.000000   
        (pInstrumentStatus->UpperLimitPrice == DBL_MAX) ? sprintf(cFields[37],"-")    : sprintf(cFields[37],"%f", pInstrumentStatus->UpperLimitPrice      );  ///涨停板价 TUstpFtdcPriceType                  0.000000   
     (pInstrumentStatus->PreSettlementPrice == DBL_MAX) ? sprintf(cFields[38],"-")    : sprintf(cFields[38],"%f", pInstrumentStatus->PreSettlementPrice   );  ///昨结算 TUstpFtdcPriceType                    0.000000  
                                                                                        sprintf(cFields[39],"%c", pInstrumentStatus->InstrumentStatus     );  ///合约交易状态 TUstpFtdcInstrumentStatusType   6
                                                                                        sprintf(cFields[40],"%s", pInstrumentStatus->CreateDate           );  ///创建日 TUstpFtdcDateType                       
                                                                                        sprintf(cFields[41],"%s", pInstrumentStatus->OpenDate             );  ///上市日 TUstpFtdcDateType                         
                                                                                        sprintf(cFields[42],"%s", pInstrumentStatus->ExpireDate           );  ///到期日 TUstpFtdcDateType                         
                                                                                        sprintf(cFields[43],"%s", pInstrumentStatus->StartDelivDate       );  ///开始交割日 TUstpFtdcDateType                     
                                                                                        sprintf(cFields[44],"%s", pInstrumentStatus->EndDelivDate         );  ///最后交割日 TUstpFtdcDateType                       
        (pInstrumentStatus->BasisPrice      == DBL_MAX) ? sprintf(cFields[45],"-")    : sprintf(cFields[45],"%f", pInstrumentStatus->BasisPrice           );  ///挂牌基准价 TUstpFtdcPriceType                0.000000  
                                                                                        sprintf(cFields[46],"%d", pInstrumentStatus->IsTrading            );  ///当前是否交易 TUstpFtdcBoolType               0  
                                                                                        sprintf(cFields[47],"%s", pInstrumentStatus->UnderlyingInstrID    );  ///基础商品代码 TUstpFtdcInstrumentIDType         
                                                                                        sprintf(cFields[48],"%d", pInstrumentStatus->UnderlyingMultiple   );  ///基础商品乘数 TUstpFtdcUnderlyingMultipleType 0    
                                                                                        sprintf(cFields[49],"%c", pInstrumentStatus->PositionType         );  ///持仓类型 TUstpFtdcPositionTypeType           0.000000  
        (pInstrumentStatus->StrikePrice     == DBL_MAX) ? sprintf(cFields[50],"-")    : sprintf(cFields[50],"%f", pInstrumentStatus->StrikePrice          );  ///执行价 TUstpFtdcPriceType                       
                                                                                        sprintf(cFields[51],"%c", pInstrumentStatus->OptionsType          );  ///期权类型 TUstpFtdcOptionsTypeType            1872923327
                                                                                        sprintf(cFields[52],"%s", pInstrumentStatus->CurrencyID           );  ///币种代码 TUstpFtdcCurrencyIDType             0.000000     
                                                                                        sprintf(cFields[53],"%c", pInstrumentStatus->ArbiType             );  ///策略类别 TUstpFtdcArbiTypeType               1872923368
                                                                                        sprintf(cFields[54],"%d", pInstrumentStatus->InstrumentID_1       );  ///第一腿合约代码 TUstpFtdcInstrumentIDType     0.000000      
                                                                                        sprintf(cFields[55],"%c", pInstrumentStatus->Direction_1          );  ///第一腿买卖方向 TUstpFtdcDirectionType         
        (pInstrumentStatus->Ratio_1         == DBL_MAX) ? sprintf(cFields[56],"-")    : sprintf(cFields[56],"%f", pInstrumentStatus->Ratio_1              );  ///第一腿数量比例 TUstpFtdcRatioType             
                                                                                        sprintf(cFields[57],"%d", pInstrumentStatus->InstrumentID_2       );  ///第二腿合约代码 TUstpFtdcInstrumentIDType         
                                                                                        sprintf(cFields[58],"%c", pInstrumentStatus->Direction_2          );  ///第二腿买卖方向 TUstpFtdcDirectionType         
        (pInstrumentStatus->Ratio_2         == DBL_MAX) ? sprintf(cFields[60],"-")    : sprintf(cFields[59],"%f", pInstrumentStatus->Ratio_2              );  ///第二腿数量比例 TUstpFtdcRatioType             
                                                                                        sprintf(cFields[60],"%s", pInstrumentStatus->EnterDate            );  ///进入本状态日期 TUstpFtdcDateType                 

        send_sline_spi_output(cFields, 61); 
    }

    ///账户出入金回报
    void TspiSimpleHandler::OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {
        printf("trr26:OnRtnInvestorAccountDeposit():w\n"); 
    }

    ///报价回报
    void TspiSimpleHandler::OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {
        printf("trr27:OnRtnQuote():w\n"); 
    }

    ///报价录入错误回报
    void TspiSimpleHandler::OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {
        printf("trr28:OnErrRtnQuoteInsert():");
        printf("ErrorCode=[%d],ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    }

    ///报价撤单错误回报
    void TspiSimpleHandler::OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo) {
        printf("trr29:OnErrRtnQuoteAction():");
        printf("ErrorCode=[%d],ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    }

    // trr30 OnRtnForQuote() 询价回报 www
    // 這個單純測試, 可以不用
    void TspiSimpleHandler::OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {
        ttTime tt_data = get_currentTT();
        //printf("%s,trr30,OnRtnForQuote(),www,InstrumentID=[%s]\n", tt_data.datentime_micro, pReqForQuote->InstrumentID);  //www
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr30"                 );   
        sprintf(cFields[ 3], "%s", "OnRtnForQuote()"  );
        sprintf(cFields[ 4], "%s", "trtn"                  );            

        sprintf(cFields[20],"%s", pReqForQuote->ReqForQuoteID   ); ///询价编号 TUstpFtdcQuoteSysIDType
        sprintf(cFields[21],"%s", pReqForQuote->BrokerID        ); ///经纪公司编号 TUstpFtdcBrokerIDType    
        sprintf(cFields[22],"%s", pReqForQuote->ExchangeID      ); ///交易所代码 TUstpFtdcExchangeIDType     
        sprintf(cFields[23],"%s", pReqForQuote->InvestorID      ); ///投资者编号 TUstpFtdcInvestorIDType
        sprintf(cFields[24],"%s", pReqForQuote->UserID          ); ///用户代码 TUstpFtdcUserIDType
        sprintf(cFields[25],"%s", pReqForQuote->InstrumentID    ); ///合约代码 TUstpFtdcInstrumentIDType    
        sprintf(cFields[26],"%c", pReqForQuote->Direction       ); ///买卖方向 TUstpFtdcDirectionType  
        sprintf(cFields[27],"%s", pReqForQuote->TradingDay      ); ///交易日 TUstpFtdcDateType
        sprintf(cFields[28],"%s", pReqForQuote->ReqForQuoteTime ); ///询价时间 TUstpFtdcTimeType        
        sprintf(cFields[29],"%s", pReqForQuote->ClientID        ); ///客户编码 TUstpFtdcClientIDType
    
        send_sline_spi_output(cFields, 30);

    }

    ///增加组合规则通知
    void TspiSimpleHandler::OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg) {
        printf("trr31:OnRtnMarginCombinationLeg():w\n"); 
    }

    ///客户申请组合确认
    void TspiSimpleHandler::OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction) {
        printf("trr32:OnRtnMarginCombAction():w\n"); 
    }

    ///用户请求出入金
    void TspiSimpleHandler::OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit) {
        printf("trr33:OnRtnUserDeposit():w\n"); 
    }

    ///查询前置系统用户登录应答
    void TspiSimpleHandler::OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr34:OnRspQueryUserLogin():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    // trr35 OnRspQryOrder() 报单查询应答
    void TspiSimpleHandler::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr35,OnRspQryOrder(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        // 萬一今天還沒有下單 會 Segmentation fault (因為 沒有回傳內容, NULL)
        if (pOrder == NULL) {
            printf("                          ,trr35:EMPTY\n");
            
            char *cFields[100];
            for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
            for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
            sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
            sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
            sprintf(cFields[ 2], "%s", "trr35"                 );   
            sprintf(cFields[ 3], "%s", "OnRspQryOrder()"    );
            sprintf(cFields[ 4], "%s", "trsp"                  );            
            sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
            sprintf(cFields[ 6], "%s", uErrorMsg               );
            sprintf(cFields[ 7], "%d", nRequestID              );
            sprintf(cFields[ 8], "%d", bIsLast                 );
            //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );   
            sprintf(cFields[10], "%s", "EMPTY"               );
            send_sline_spi_output(cFields, 20);
            return;    
        }
        
        printf("                          ,trr35:OrderSysID=[%s],UserOrderLocalID=[%s]\n", pOrder->OrderSysID, pOrder->UserOrderLocalID);
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr35"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryOrder()"    );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
        //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );    
        //send_sline_spi_output(cFields, 32);
                                                                        sprintf(cFields[20],"%s", pOrder->BrokerID         ); // 0059              0059           0059             ///经纪公司编号             TUstpFtdcBrokerIDType        
                                                                        sprintf(cFields[21],"%s", pOrder->ExchangeID       ); // CFFEX             CFFEX          CFFEX            ///交易所代码               TUstpFtdcExchangeIDType      
                                                                        sprintf(cFields[22],"%s", pOrder->OrderSysID       ); //      1090502                           298799     ///系统报单编号             TUstpFtdcOrderSysIDType      
                                                                        sprintf(cFields[23],"%s", pOrder->InvestorID       ); // 990407            990407         990407           ///投资者编号               TUstpFtdcInvestorIDType      
                                                                        sprintf(cFields[24],"%s", pOrder->UserID           ); // 990407            990407         990407           ///用户代码                 TUstpFtdcUserIDType             
                                                                        sprintf(cFields[25],"%d", pOrder->SeatNo           ); // 0                 0              0                ///指定通过此席位序号下单   TUstpFtdcSeatNoType          
                                                                        sprintf(cFields[26],"%s", pOrder->InstrumentID     ); // IO1904-C-3800     IO1904-C-3800  IO1904-C-3800    ///合约代码/套利组合合约号  TUstpFtdcInstrumentIDType    
                                                                        sprintf(cFields[27],"%s", pOrder->UserOrderLocalID ); // 1555308830436637  15553029150878 1555641113059647 ///用户本地报单号           TUstpFtdcUserOrderLocalIDType
                                                                        sprintf(cFields[28],"%c", pOrder->OrderPriceType   ); // 2                 2              2                ///报单类型                 TUstpFtdcOrderPriceTypeType    
                                                                        sprintf(cFields[29],"%c", pOrder->Direction        ); // 1                 1              0                ///买卖方向                 TUstpFtdcDirectionType        
                                                                        sprintf(cFields[30],"%c", pOrder->OffsetFlag       ); // 1                 0              1                ///开平标志                 TUstpFtdcOffsetFlagType        
                                                                        sprintf(cFields[31],"%c", pOrder->HedgeFlag        ); // 1                 1              1                ///投机套保标志             TUstpFtdcHedgeFlagType       
        (pOrder->LimitPrice == DBL_MAX) ? sprintf(cFields[32],"-")    : sprintf(cFields[32],"%f", pOrder->LimitPrice       ); // 600.000000        100.000000     600.000000       ///价格                     TUstpFtdcPriceType              
                                                                        sprintf(cFields[33],"%d", pOrder->Volume           ); // 5                 1              10               ///数量                     TUstpFtdcVolumeType              
                                                                        sprintf(cFields[34],"%c", pOrder->TimeCondition    ); // 3                 3              3                ///有效期类型               TUstpFtdcTimeConditionType   
                                                                        sprintf(cFields[35],"%s", pOrder->GTDDate          ); // 20150801                         20150802         ///GTD日期                  TUstpFtdcDateType            
                                                                        sprintf(cFields[36],"%c", pOrder->VolumeCondition  ); // 1                 1              1                ///成交量类型               TUstpFtdcVolumeConditionType   
                                                                        sprintf(cFields[37],"%d", pOrder->MinVolume        ); // 0                 0              0                ///最小成交量               TUstpFtdcVolumeType           
        (pOrder->StopPrice  == DBL_MAX) ? sprintf(cFields[38],"-")    : sprintf(cFields[38],"%f", pOrder->StopPrice        ); // 0.000000          0.000000       0.000000         ///止损价止赢价             TUstpFtdcPriceType           
                                                                        sprintf(cFields[39],"%c", pOrder->ForceCloseReason ); // 0                 0              0                ///强平原因                 TUstpFtdcForceCloseReasonType 
                                                                        sprintf(cFields[40],"%d", pOrder->IsAutoSuspend    ); // 0                 0              0                ///自动挂起标志             TUstpFtdcBoolType            
                                                                        sprintf(cFields[41],"%s", pOrder->BusinessUnit     ); // LoopSample        LoopSample     LoopSample       ///业务单元                 TUstpFtdcBusinessUnitType     
                                                                        sprintf(cFields[42],"%s", pOrder->UserCustom       ); // self                 45314378390 self             ///用户自定义域             TUstpFtdcCustomType          
                                                                        sprintf(cFields[43],"%d", pOrder->BusinessLocalID  ); // 0                 0              0                ///本地业务标识             TUstpFtdcBusinessLocalIDType 
                                                                        sprintf(cFields[44],"%s", pOrder->ActionDay        ); //                                                   ///业务发生日期             TUstpFtdcDateType            
                                                                        sprintf(cFields[45],"%c", pOrder->ArbiType         ); // 0                 0              0                ///策略类别                 TUstpFtdcArbiTypeType        
                                                                        sprintf(cFields[46],"%s", pOrder->TradingDay       ); // 20190415          20190415       20190419         ///交易日                   TUstpFtdcTradingDayType           
                                                                        sprintf(cFields[47],"%s", pOrder->ParticipantID    ); // 2034              2034           2034             ///会员编号                 TUstpFtdcParticipantIDType     
                                                                        sprintf(cFields[48],"%s", pOrder->OrderUserID      ); // 990407            990407         990407           ///最初下单用户编号         TUstpFtdcUserIDType          
                                                                        sprintf(cFields[49],"%s", pOrder->ClientID         ); // 01568909          01568909       01568909         ///客户编码                 TUstpFtdcClientIDType        
                                                                        sprintf(cFields[50],"%s", pOrder->SeatID           ); // 203403            203403         203404           ///下单席位号               TUstpFtdcSeatIDType          
                                                                        sprintf(cFields[51],"%s", pOrder->InsertTime       ); // 14:13:50          12:35:14       10:31:53         ///插入时间                 TUstpFtdcTimeType            
                                                                        sprintf(cFields[52],"%s", pOrder->OrderLocalID     ); // 000000000004      000000000001   000000000001     ///本地报单编号             TUstpFtdcOrderLocalIDType    
                                                                        sprintf(cFields[53],"%c", pOrder->OrderSource      ); // 0                 0              0                ///报单来源                 TUstpFtdcOrderSourceType     
                                                                        sprintf(cFields[54],"%c", pOrder->OrderStatus      ); // 3                 4              5                ///报单状态                 TUstpFtdcOrderStatusType     
                                                                        sprintf(cFields[55],"%s", pOrder->CancelTime       ); //                                  10:36:05         ///撤销时间                 TUstpFtdcTimeType            
                                                                        sprintf(cFields[56],"%s", pOrder->CancelUserID     ); //                                                   ///撤单用户编号             TUstpFtdcUserIDType          
                                                                        sprintf(cFields[57],"%d", pOrder->VolumeTraded     ); // 0                 0              6                ///今成交数量               TUstpFtdcVolumeType          
                                                                        sprintf(cFields[58],"%d", pOrder->VolumeRemain     ); // 5                 1              4                ///剩余数量                 TUstpFtdcVolumeType            
                                                                        sprintf(cFields[59],"%c", pOrder->OrderType        ); // 0                 0              0                ///委托类型                 TUstpFtdcOrderTypeType        
                                                                        sprintf(cFields[60],"%c", pOrder->DeliveryFlag     ); // 0                 0              0                ///期权对冲标识             TUstpFtdcDeliveryFlagType    
        send_sline_spi_output(cFields, 61);

    }

    // trr36 OnRspQryTrade() 成交单查询应答
    void TspiSimpleHandler::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr36,OnRspQryTrade(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        // 萬一今天還沒有成交單 會 Segmentation fault (因為 沒有回傳內容, NULL)
        if (pTrade == NULL) {
            printf("                          ,trr36:EMPTY\n");
            char *cFields[100];
            for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
            for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
            sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
            sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
            sprintf(cFields[ 2], "%s", "trr36"                 );   
            sprintf(cFields[ 3], "%s", "OnRspQryTrade()"    );
            sprintf(cFields[ 4], "%s", "trsp"                  );            
            sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
            sprintf(cFields[ 6], "%s", uErrorMsg               );
            sprintf(cFields[ 7], "%d", nRequestID              );
            sprintf(cFields[ 8], "%d", bIsLast                 );
            //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );   
            sprintf(cFields[10], "%s", "EMPTY"               );
            send_sline_spi_output(cFields, 20);
            return;    
        }

        printf("                          ,trr36:OrderSysID=[%s],TradeID=[%s],TradeVolume=[%d]\n", pTrade->OrderSysID, pTrade->TradeID, pTrade->TradeVolume);
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr36"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryTrade()"    );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
        //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );    
                                                                        sprintf(cFields[20],"%s", pTrade->BrokerID         ); ///经纪公司编号        TUstpFtdcBrokerIDType
                                                                        sprintf(cFields[21],"%s", pTrade->ExchangeID       ); ///交易所代码          TUstpFtdcExchangeIDType
                                                                        sprintf(cFields[22],"%s", pTrade->TradingDay       ); ///交易日              TUstpFtdcTradingDayType
                                                                        sprintf(cFields[23],"%s", pTrade->ParticipantID    ); ///会员编号            TUstpFtdcParticipantIDType
                                                                        sprintf(cFields[24],"%s", pTrade->SeatID           ); ///下单席位号          TUstpFtdcSeatIDType
                                                                        sprintf(cFields[25],"%s", pTrade->InvestorID       ); ///投资者编号          TUstpFtdcInvestorIDType
                                                                        sprintf(cFields[26],"%s", pTrade->ClientID         ); ///客户编码            TUstpFtdcClientIDType
                                                                        sprintf(cFields[27],"%s", pTrade->UserID           ); ///用户编号            TUstpFtdcUserIDType
                                                                        sprintf(cFields[28],"%s", pTrade->OrderUserID      ); ///下单用户编号        TUstpFtdcUserIDType
                                                                        sprintf(cFields[29],"%s", pTrade->TradeID          ); ///成交编号            TUstpFtdcTradeIDType
                                                                        sprintf(cFields[30],"%s", pTrade->OrderSysID       ); ///系统报单编号        TUstpFtdcOrderSysIDType
                                                                        sprintf(cFields[31],"%s", pTrade->UserOrderLocalID ); ///本地报单编号        TUstpFtdcUserOrderLocalIDType
                                                                        sprintf(cFields[32],"%s", pTrade->InstrumentID     ); ///合约代码            TUstpFtdcInstrumentIDType
                                                                        sprintf(cFields[33],"%c", pTrade->Direction        ); ///买卖方向            TUstpFtdcDirectionType
                                                                        sprintf(cFields[34],"%c", pTrade->OffsetFlag       ); ///开平标志            TUstpFtdcOffsetFlagType
                                                                        sprintf(cFields[35],"%c", pTrade->HedgeFlag        ); ///投机套保标志        TUstpFtdcHedgeFlagType
        (pTrade->TradePrice == DBL_MAX) ?   sprintf(cFields[36],"-")  : sprintf(cFields[36],"%f", pTrade->TradePrice       ); ///成交价格            TUstpFtdcPriceType
                                                                        sprintf(cFields[37],"%d", pTrade->TradeVolume      ); ///成交数量            TUstpFtdcVolumeType
                                                                        sprintf(cFields[38],"%s", pTrade->TradeTime        ); ///成交时间            TUstpFtdcTimeType
                                                                        sprintf(cFields[39],"%s", pTrade->ClearingPartID   ); ///清算会员编号        TUstpFtdcParticipantIDType
        (pTrade->UsedFee    == DBL_MAX) ?   sprintf(cFields[40],"-")  : sprintf(cFields[40],"%f", pTrade->UsedFee          ); ///本次成交手续费      TUstpFtdcMoneyType
        (pTrade->UsedMargin == DBL_MAX) ?   sprintf(cFields[41],"-")  : sprintf(cFields[41],"%f", pTrade->UsedMargin       ); ///本次成交占用保证金  TUstpFtdcMoneyType
        (pTrade->Premium    == DBL_MAX) ?   sprintf(cFields[42],"-")  : sprintf(cFields[42],"%f", pTrade->Premium          ); ///本次成交占用权利金  TUstpFtdcMoneyType
                                                                        sprintf(cFields[43],"%d", pTrade->Position         ); ///持仓表今持仓量      TUstpFtdcVolumeType
        (pTrade->PositionCost == DBL_MAX) ? sprintf(cFields[44],"-")  : sprintf(cFields[44],"%f", pTrade->PositionCost     ); ///持仓表今日持仓成本  TUstpFtdcPriceType
        (pTrade->Available    == DBL_MAX) ? sprintf(cFields[45],"-")  : sprintf(cFields[45],"%f", pTrade->Available        ); ///资金表可用资金      TUstpFtdcMoneyType
        (pTrade->Margin       == DBL_MAX) ? sprintf(cFields[46],"-")  : sprintf(cFields[46],"%f", pTrade->Margin           ); ///资金表占用保证金    TUstpFtdcMoneyType
        (pTrade->FrozenMargin == DBL_MAX) ? sprintf(cFields[47],"-")  : sprintf(cFields[47],"%f", pTrade->FrozenMargin     ); ///资金表冻结的保证金  TUstpFtdcMoneyType
                                                                        sprintf(cFields[48],"%d", pTrade->BusinessLocalID  ); ///本地业务标识        TUstpFtdcBusinessLocalIDType
                                                                        sprintf(cFields[49],"%s", pTrade->ActionDay        ); ///业务发生日期        TUstpFtdcDateType
                                                                        sprintf(cFields[50],"%c", pTrade->ArbiType         ); ///策略类别            TUstpFtdcArbiTypeType
                                                                        sprintf(cFields[51],"%s", pTrade->ArbiInstrumentID ); ///套利组合合约        TUstpFtdcInstrumentIDType
        send_sline_spi_output(cFields, 52);

    }

    // trr37 OnRspQryUserInvestor() 投资者资金账户查询请求应答
    void TspiSimpleHandler::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr37,OnRspQryUserInvestor(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        printf("                          ,trr37:InvestorID=[%s]\n", pRspUserInvestor->InvestorID);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr37"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryUserInvestor()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 
        
        sprintf(cFields[20],"%s", pRspUserInvestor->BrokerID    );   ///经纪公司编号 TUstpFtdcBrokerIDType
        sprintf(cFields[21],"%s", pRspUserInvestor->UserID      );   ///交易用户代码 TUstpFtdcUserIDType
        sprintf(cFields[22],"%s", pRspUserInvestor->InvestorID  );   ///投资者编号   TUstpFtdcInvestorIDType
        
        send_sline_spi_output(cFields, 23);
    }
    
    // trr38 OnRspQryTradingCode() 交易编码查询应答
    void TspiSimpleHandler::OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
            
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr38,OnRspQryTradingCode(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        printf("                          ,trr38:ClientRight=[%c],ClientHedgeFlag=[%c],IsActive=[%c]\n", pRspTradingCode->ClientRight, pRspTradingCode->ClientHedgeFlag, pRspTradingCode->IsActive);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr38"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryTradingCode()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

        sprintf(cFields[20],"%s", pRspTradingCode->BrokerID         );   ///经纪公司编号 TUstpFtdcBrokerIDType           
        sprintf(cFields[21],"%s", pRspTradingCode->ExchangeID       );   ///交易所代码   TUstpFtdcExchangeIDType           
        sprintf(cFields[22],"%s", pRspTradingCode->InvestorID       );   ///投资者编号   TUstpFtdcInvestorIDType           
        sprintf(cFields[23],"%s", pRspTradingCode->ClientID         );   ///客户编码     TUstpFtdcClientIDType               
        sprintf(cFields[24],"%c", pRspTradingCode->ClientRight      );   ///客户编码权限 TUstpFtdcTradingRightType        
        sprintf(cFields[25],"%c", pRspTradingCode->ClientHedgeFlag  );   ///客户保值类型 TUstpFtdcHedgeFlagType            
        sprintf(cFields[26],"%c", pRspTradingCode->IsActive         );   ///是否活跃     TUstpFtdcIsActiveType                
        
        send_sline_spi_output(cFields, 27);
            
    }

    // trr39 OnRspQryInvestorAccount() 投资者资金账户查询应答 // much info
    void TspiSimpleHandler::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr39,OnRspQryInvestorAccount(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        printf("                          ,trr39:Risk=[%f]\n", pRspInvestorAccount->Risk);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr39"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryInvestorAccount()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );         

                                                                                             sprintf(cFields[20],"%s", pRspInvestorAccount->BrokerID          ); ///经纪公司编号     0059            TUstpFtdcBrokerIDType   
                                                                                             sprintf(cFields[21],"%s", pRspInvestorAccount->InvestorID        ); ///投资者编号       990407          TUstpFtdcInvestorIDType   
                                                                                             sprintf(cFields[22],"%s", pRspInvestorAccount->AccountID         ); ///资金帐号         990407          TUstpFtdcAccountIDType     
        (pRspInvestorAccount->PreBalance          == DBL_MAX) ? sprintf(cFields[23],"-")   : sprintf(cFields[23],"%f", pRspInvestorAccount->PreBalance        ); ///上次结算准备金   24998813.140000 TUstpFtdcMoneyType      
        (pRspInvestorAccount->Deposit             == DBL_MAX) ? sprintf(cFields[24],"-")   : sprintf(cFields[24],"%f", pRspInvestorAccount->Deposit           ); ///入金金额         0.000000        TUstpFtdcMoneyType         
        (pRspInvestorAccount->Withdraw            == DBL_MAX) ? sprintf(cFields[25],"-")   : sprintf(cFields[25],"%f", pRspInvestorAccount->Withdraw          ); ///出金金额         0.000000        TUstpFtdcMoneyType         
        (pRspInvestorAccount->FrozenMargin        == DBL_MAX) ? sprintf(cFields[26],"-")   : sprintf(cFields[26],"%f", pRspInvestorAccount->FrozenMargin      ); ///冻结的保证金     0.000000        TUstpFtdcMoneyType       
        (pRspInvestorAccount->FrozenFee           == DBL_MAX) ? sprintf(cFields[27],"-")   : sprintf(cFields[27],"%f", pRspInvestorAccount->FrozenFee         ); ///冻结手续费       0.000000        TUstpFtdcMoneyType        
        (pRspInvestorAccount->FrozenPremium       == DBL_MAX) ? sprintf(cFields[28],"-")   : sprintf(cFields[28],"%f", pRspInvestorAccount->FrozenPremium     ); ///冻结权利金       0.000000        TUstpFtdcMoneyType        
        (pRspInvestorAccount->Fee                 == DBL_MAX) ? sprintf(cFields[29],"-")   : sprintf(cFields[29],"%f", pRspInvestorAccount->Fee               ); ///手续费           0.000000        TUstpFtdcMoneyType              
        (pRspInvestorAccount->CloseProfit         == DBL_MAX) ? sprintf(cFields[30],"-")   : sprintf(cFields[30],"%f", pRspInvestorAccount->CloseProfit       ); ///平仓盈亏         0.000000        TUstpFtdcMoneyType         
        (pRspInvestorAccount->PositionProfit      == DBL_MAX) ? sprintf(cFields[31],"-")   : sprintf(cFields[31],"%f", pRspInvestorAccount->PositionProfit    ); ///持仓盈亏 (變)    -9840.000000    TUstpFtdcMoneyType         
        (pRspInvestorAccount->Available           == DBL_MAX) ? sprintf(cFields[32],"-")   : sprintf(cFields[32],"%f", pRspInvestorAccount->Available         ); ///可用资金 (變)    24447378.340000 TUstpFtdcMoneyType         
        (pRspInvestorAccount->LongFrozenMargin    == DBL_MAX) ? sprintf(cFields[33],"-")   : sprintf(cFields[33],"%f", pRspInvestorAccount->LongFrozenMargin  ); ///多头冻结的保证金 0.000000        TUstpFtdcMoneyType      
        (pRspInvestorAccount->ShortFrozenMargin   == DBL_MAX) ? sprintf(cFields[34],"-")   : sprintf(cFields[34],"%f", pRspInvestorAccount->ShortFrozenMargin ); ///空头冻结的保证金 0.000000        TUstpFtdcMoneyType      
        (pRspInvestorAccount->LongMargin          == DBL_MAX) ? sprintf(cFields[35],"-")   : sprintf(cFields[35],"%f", pRspInvestorAccount->LongMargin        ); ///多头占用保证金   541594.800000   TUstpFtdcMoneyType      
        (pRspInvestorAccount->ShortMargin         == DBL_MAX) ? sprintf(cFields[36],"-")   : sprintf(cFields[36],"%f", pRspInvestorAccount->ShortMargin       ); ///空头占用保证金   0.000000        TUstpFtdcMoneyType      
        (pRspInvestorAccount->ReleaseMargin       == DBL_MAX) ? sprintf(cFields[37],"-")   : sprintf(cFields[37],"%f", pRspInvestorAccount->ReleaseMargin     ); ///当日释放保证金   0.000000        TUstpFtdcMoneyType      
        (pRspInvestorAccount->DynamicRights       == DBL_MAX) ? sprintf(cFields[38],"-")   : sprintf(cFields[38],"%f", pRspInvestorAccount->DynamicRights     ); ///动态权益         0.000000        TUstpFtdcMoneyType            
        (pRspInvestorAccount->TodayInOut          == DBL_MAX) ? sprintf(cFields[39],"-")   : sprintf(cFields[39],"%f", pRspInvestorAccount->TodayInOut        ); ///今日出入金       0.000000        TUstpFtdcMoneyType           
        (pRspInvestorAccount->Margin              == DBL_MAX) ? sprintf(cFields[40],"-")   : sprintf(cFields[40],"%f", pRspInvestorAccount->Margin            ); ///占用保证金       541594.800000   TUstpFtdcMoneyType           
        (pRspInvestorAccount->Premium             == DBL_MAX) ? sprintf(cFields[41],"-")   : sprintf(cFields[41],"%f", pRspInvestorAccount->Premium           ); ///期权权利金收支   0.000000        TUstpFtdcMoneyType      
        (pRspInvestorAccount->Risk                == DBL_MAX) ? sprintf(cFields[42],"-")   : sprintf(cFields[42],"%f", pRspInvestorAccount->Risk              ); ///风险度           2.170000        TUstpFtdcMoneyType             
                                                                                                             
        send_sline_spi_output(cFields, 43);                                                                  
    }
    
    // trr40 OnRspQryInstrument 合约查询应答
    void TspiSimpleHandler::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr40,OnRspQryInstrument(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);
        // 萬一沒有內容 會 Segmentation fault (因為 沒有回傳內容, NULL)
        if (pRspInstrument == NULL) {
            printf("                          ,trr40:EMPTY\n");
            char *cFields[100];
            for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
            for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
            sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
            sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
            sprintf(cFields[ 2], "%s", "trr40"                 );   
            sprintf(cFields[ 3], "%s", "OnRspQryInstrument()"  );
            sprintf(cFields[ 4], "%s", "trsp"                  );            
            sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
            sprintf(cFields[ 6], "%s", uErrorMsg               );
            sprintf(cFields[ 7], "%d", nRequestID              );
            sprintf(cFields[ 8], "%d", bIsLast                 );
            sprintf(cFields[10], "%s", "EMPTY"                 );

            send_sline_spi_output(cFields, 20);
            return;    
        }

        printf("                          ,trr40:InstrumentID:%s,LowerLimitPrice:%f,UpperLimitPrice:%f\n", pRspInstrument->InstrumentID, pRspInstrument->LowerLimitPrice, pRspInstrument->UpperLimitPrice);
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr40"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryInstrument()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 
        // template
        //sprintf(cFields[20],"%s", pRspInstrument->ExchangeID           );
        //send_sline_spi_output(cFields, 60);

        char uProductName[200];
        memset(&uProductName, 0, sizeof(uProductName));
        my_iconv("GB2312", "UTF-8", pRspInstrument->ProductName, uProductName); // (from, to, in, out)
        char uInstrumentName[200];
        memset(&uInstrumentName, 0, sizeof(uInstrumentName));
        my_iconv("GB2312", "UTF-8", pRspInstrument->InstrumentName, uInstrumentName); // (from, to, in, out)
        
                                                                                    sprintf(cFields[20],"%s", pRspInstrument->ExchangeID           );  ///交易所代码       CFFEX                TUstpFtdcExchangeIDType          
                                                                                    sprintf(cFields[21],"%s", pRspInstrument->ProductID            );  ///品种代码         IO                   TUstpFtdcProductIDType           
                                                                                    sprintf(cFields[22],"%s", uProductName                         );  ///品种名称                              TUstpFtdcProductNameType         
                                                                                    sprintf(cFields[23],"%s", pRspInstrument->InstrumentID         );  ///合约代码         IO1904-C-4000        TUstpFtdcInstrumentIDType        
                                                                                    sprintf(cFields[24],"%s", uInstrumentName                      );  ///合约名称         股指期权1904-C-4000  TUstpFtdcInstrumentNameType      
                                                                                    sprintf(cFields[25],"%d", pRspInstrument->DeliveryYear         );  ///交割年份         2019                 TUstpFtdcYearType                 
                                                                                    sprintf(cFields[26],"%d", pRspInstrument->DeliveryMonth        );  ///交割月           4                    TUstpFtdcMonthType                    
                                                                                    sprintf(cFields[27],"%d", pRspInstrument->MaxLimitOrderVolume  );  ///限价单最大下单量 100                  TUstpFtdcVolumeType              
                                                                                    sprintf(cFields[28],"%d", pRspInstrument->MinLimitOrderVolume  );  ///限价单最小下单量 1                    TUstpFtdcVolumeType              
                                                                                    sprintf(cFields[29],"%d", pRspInstrument->MaxMarketOrderVolume );  ///市价单最大下单量 0                    TUstpFtdcVolumeType              
                                                                                    sprintf(cFields[30],"%d", pRspInstrument->MinMarketOrderVolume );  ///市价单最小下单量 1                    TUstpFtdcVolumeType              
                                                                                    sprintf(cFields[31],"%d", pRspInstrument->VolumeMultiple       );  ///数量乘数         100                  TUstpFtdcVolumeMultipleType      
        (pRspInstrument->PriceTick       == DBL_MAX) ? sprintf(cFields[32],"-")   : sprintf(cFields[32],"%f", pRspInstrument->PriceTick            );  ///报价单位         0.200000             TUstpFtdcPriceTickType           
                                                                                    sprintf(cFields[33],"%c", pRspInstrument->Currency             );  ///币种             1                    TUstpFtdcCurrencyType                 
                                                                                    sprintf(cFields[34],"%d", pRspInstrument->LongPosLimit         );  ///多头限仓         0                    TUstpFtdcVolumeType              
                                                                                    sprintf(cFields[35],"%d", pRspInstrument->ShortPosLimit        );  ///空头限仓         0                    TUstpFtdcVolumeType              
        (pRspInstrument->LowerLimitPrice == DBL_MAX) ? sprintf(cFields[36],"-")   : sprintf(cFields[36],"%f", pRspInstrument->LowerLimitPrice      );  ///跌停板价         0.200000             TUstpFtdcPriceType               
        (pRspInstrument->UpperLimitPrice == DBL_MAX) ? sprintf(cFields[37],"-")   : sprintf(cFields[37],"%f", pRspInstrument->UpperLimitPrice      );  ///涨停板价         489.400000           TUstpFtdcPriceType               
     (pRspInstrument->PreSettlementPrice == DBL_MAX) ? sprintf(cFields[38],"-")   : sprintf(cFields[38],"%f", pRspInstrument->PreSettlementPrice   );  ///昨结算           89.800000            TUstpFtdcPriceType               
                                                                                    sprintf(cFields[39],"%c", pRspInstrument->InstrumentStatus     );  ///合约交易状态     1                    TUstpFtdcInstrumentStatusType    
                                                                                    sprintf(cFields[40],"%s", pRspInstrument->CreateDate           );  ///创建日           20190225             TUstpFtdcDateType                
                                                                                    sprintf(cFields[41],"%s", pRspInstrument->OpenDate             );  ///上市日           20190226             TUstpFtdcDateType                
                                                                                    sprintf(cFields[42],"%s", pRspInstrument->ExpireDate           );  ///到期日           20190419             TUstpFtdcDateType                
                                                                                    sprintf(cFields[43],"%s", pRspInstrument->StartDelivDate       );  ///开始交割日       20190419             TUstpFtdcDateType                
                                                                                    sprintf(cFields[44],"%s", pRspInstrument->EndDelivDate         );  ///最后交割日       20190419             TUstpFtdcDateType                
        (pRspInstrument->BasisPrice      == DBL_MAX) ? sprintf(cFields[45],"-")   : sprintf(cFields[45],"%f", pRspInstrument->BasisPrice           );  ///挂牌基准价       29.000000            TUstpFtdcPriceType               
                                                                                    sprintf(cFields[46],"%d", pRspInstrument->IsTrading            );  ///当前是否交易     1                    TUstpFtdcBoolType                
                                                                                    sprintf(cFields[47],"%s", pRspInstrument->UnderlyingInstrID    );  ///基础商品代码     IO1904               TUstpFtdcInstrumentIDType        
                                                                                    sprintf(cFields[48],"%d", pRspInstrument->UnderlyingMultiple   );  ///基础商品乘数     1                    TUstpFtdcUnderlyingMultipleType  
                                                                                    sprintf(cFields[49],"%c", pRspInstrument->PositionType         );  ///持仓类型         2                    TUstpFtdcPositionTypeType        
        (pRspInstrument->StrikePrice     == DBL_MAX) ? sprintf(cFields[50],"-")   : sprintf(cFields[50],"%f", pRspInstrument->StrikePrice          );  ///执行价           4000.000000          TUstpFtdcPriceType               
                                                                                    sprintf(cFields[51],"%c", pRspInstrument->OptionsType          );  ///期权类型         1                    TUstpFtdcOptionsTypeType         
                                                                                    sprintf(cFields[52],"%s", pRspInstrument->CurrencyID           );  ///币种代码                              TUstpFtdcCurrencyIDType          
                                                                                    sprintf(cFields[53],"%c", pRspInstrument->ArbiType             );  ///策略类别         0                    TUstpFtdcArbiTypeType            
                                                                                    sprintf(cFields[54],"%d", pRspInstrument->InstrumentID_1       );  ///第一腿合约代码   1105324655           TUstpFtdcInstrumentIDType        
                                                                                    sprintf(cFields[55],"%c", pRspInstrument->Direction_1          );  ///第一腿买卖方向   0                    TUstpFtdcDirectionType           
        (pRspInstrument->Ratio_1         == DBL_MAX) ? sprintf(cFields[56],"-")   : sprintf(cFields[56],"%f", pRspInstrument->Ratio_1              );  ///第一腿数量比例   0.000000             TUstpFtdcRatioType               
                                                                                    sprintf(cFields[57],"%d", pRspInstrument->InstrumentID_2       );  ///第二腿合约代码   1105324696           TUstpFtdcInstrumentIDType        
                                                                                    sprintf(cFields[58],"%c", pRspInstrument->Direction_2          );  ///第二腿买卖方向   1                    TUstpFtdcDirectionType           
        (pRspInstrument->Ratio_2         == DBL_MAX) ? sprintf(cFields[59],"-")   : sprintf(cFields[59],"%f", pRspInstrument->Ratio_2              );  ///第二腿数量比例   0.000000             TUstpFtdcRatioType               
        

        send_sline_spi_output(cFields, 60);
    }
    
    /// OnRspQryExchange交易所查询应答
    void TspiSimpleHandler::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr41:OnRspQryExchange():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    };

    // trr42 OnRspQryInvestorPosition 投资者持仓查询应答
    void TspiSimpleHandler::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
        
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,trr42,OnRspQryInvestorPosition(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        // 沒有回傳內容會 Segmentation fault (因為 沒有回傳內容, NULL)
        if (pRspInvestorPosition == NULL) {
            printf("                          ,trr42:EMPTY\n");
            
            char *cFields[100];
            for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
            for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
            sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
            sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
            sprintf(cFields[ 2], "%s", "trr42"                 );   
            sprintf(cFields[ 3], "%s", "OnRspQryInvestorPosition()"    );
            sprintf(cFields[ 4], "%s", "trsp"                  );            
            sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
            sprintf(cFields[ 6], "%s", uErrorMsg               );
            sprintf(cFields[ 7], "%d", nRequestID              );
            sprintf(cFields[ 8], "%d", bIsLast                 );
            //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    );   
            sprintf(cFields[10], "%s", "EMPTY"               );
            send_sline_spi_output(cFields, 20);
            return;    
        }

        printf("                          ,trr42:InstrumentID:%s,Direction:%c,Position:%d\n", pRspInvestorPosition->InstrumentID, pRspInvestorPosition->Direction, pRspInvestorPosition->Position);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "trr42"                 );   
        sprintf(cFields[ 3], "%s", "OnRspQryInvestorPosition()"  );
        sprintf(cFields[ 4], "%s", "trsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

                                                                                            
                                                                                            sprintf(cFields[20],"%s", pRspInvestorPosition->InvestorID          );  /// 990407          投资者编号             TUstpFtdcInvestorIDType    
                                                                                            sprintf(cFields[21],"%s", pRspInvestorPosition->BrokerID            );  /// 0059            经纪公司编号           TUstpFtdcBrokerIDType    
                                                                                            sprintf(cFields[22],"%s", pRspInvestorPosition->ExchangeID          );  /// CFFEX           交易所代码             TUstpFtdcExchangeIDType    
                                                                                            sprintf(cFields[23],"%s", pRspInvestorPosition->ClientID            );  /// 01568909        客户编码               TUstpFtdcClientIDType        
                                                                                            sprintf(cFields[24],"%s", pRspInvestorPosition->InstrumentID        );  /// IO1905-C-3900   合约代码               TUstpFtdcInstrumentIDType    
                                                                                            sprintf(cFields[25],"%c", pRspInvestorPosition->Direction           );  /// 0               买卖方向               TUstpFtdcDirectionType        
                                                                                            sprintf(cFields[26],"%c", pRspInvestorPosition->HedgeFlag           );  /// 1               投机套保标志           TUstpFtdcHedgeFlagType    
        (pRspInvestorPosition->UsedMargin       == DBL_MAX) ? sprintf(cFields[27],"-")    : sprintf(cFields[27],"%f", pRspInvestorPosition->UsedMargin          );  /// 0.000000        优惠前占用保证金       TUstpFtdcMoneyType    
                                                                                            sprintf(cFields[28],"%d", pRspInvestorPosition->Position            );  /// 2               今总持仓量             TUstpFtdcVolumeType        
        (pRspInvestorPosition->PositionCost     == DBL_MAX) ? sprintf(cFields[29],"-")    : sprintf(cFields[29],"%f", pRspInvestorPosition->PositionCost        );  /// 41840.000000    今日持仓成本           TUstpFtdcPriceType        
                                                                                            sprintf(cFields[30],"%d", pRspInvestorPosition->YdPosition          );  /// 2               昨持仓量               TUstpFtdcVolumeType            
        (pRspInvestorPosition->YdPositionCost   == DBL_MAX) ? sprintf(cFields[31],"-")    : sprintf(cFields[31],"%f", pRspInvestorPosition->YdPositionCost      );  /// 41840.000000    昨日持仓成本           TUstpFtdcMoneyType        
        (pRspInvestorPosition->FrozenMargin     == DBL_MAX) ? sprintf(cFields[32],"-")    : sprintf(cFields[32],"%f", pRspInvestorPosition->FrozenMargin        );  /// 0.000000        冻结的保证金           TUstpFtdcMoneyType        
                                                                                            sprintf(cFields[33],"%d", pRspInvestorPosition->FrozenPosition      );  /// 0               开仓冻结持仓           TUstpFtdcVolumeType        
                                                                                            sprintf(cFields[34],"%d", pRspInvestorPosition->FrozenClosing       );  /// 0               平仓冻结持仓           TUstpFtdcVolumeType        
                                                                                            sprintf(cFields[35],"%d", pRspInvestorPosition->YdFrozenClosing     );  /// 0               平昨仓冻结持仓         TUstpFtdcVolumeType    
        (pRspInvestorPosition->FrozenPremium    == DBL_MAX) ? sprintf(cFields[36],"-")    : sprintf(cFields[36],"%f", pRspInvestorPosition->FrozenPremium       );  /// 0.000000        冻结的权利金           TUstpFtdcMoneyType        
                                                                                            sprintf(cFields[37],"%s", pRspInvestorPosition->LastTradeID         );  /// 0               最后一笔成交编号       TUstpFtdcTradeIDType
                                                                                            sprintf(cFields[38],"%s", pRspInvestorPosition->LastOrderLocalID    );  /// 0               最后一笔本地报单编号   TUstpFtdcOrderLocalIDType    
                                                                                            sprintf(cFields[39],"%d", pRspInvestorPosition->SpeculationPosition );  /// 2               投机持仓量             TUstpFtdcVolumeType                    
                                                                                            sprintf(cFields[40],"%d", pRspInvestorPosition->ArbitragePosition   );  /// 0               套利持仓量             TUstpFtdcVolumeType                    
                                                                                            sprintf(cFields[41],"%d", pRspInvestorPosition->HedgePosition       );  /// 0               套保持仓量             TUstpFtdcVolumeType                    
                                                                                            sprintf(cFields[42],"%d", pRspInvestorPosition->SpecFrozenClosing   );  /// 0               投机平仓冻结量         TUstpFtdcVolumeType                
                                                                                            sprintf(cFields[43],"%d", pRspInvestorPosition->HedgeFrozenClosing  );  /// 0               套保平仓冻结量         TUstpFtdcVolumeType                
                                                                                            sprintf(cFields[44],"%s", pRspInvestorPosition->Currency            );  /// CNY             币种                   TUstpFtdcCurrencyIDType                        
                                                                                                            
        send_sline_spi_output(cFields, 45);
        
    }
    
    /// OnRspQryComplianceParam 合规参数查询应答
    void TspiSimpleHandler::OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr43:OnRspQryComplianceParam():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    /// OnRspQryInvestorFee 投资者手续费率查询应答
    void TspiSimpleHandler::OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr44:OnRspQryInvestorFee():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
    }

    /// OnRspQryInvestorMargin 投资者保证金率查询应答
    void TspiSimpleHandler::OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr45:OnRspQryInvestorMargin():");
        char dst[200];
        memset(&dst, 0, sizeof(dst));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, dst); // (from, to, in, out)
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, dst, nRequestID, bIsLast);
        printf("trr45:LongMarginRate=[%f],LongMarginAmt=[%f],HedgeFlag=[%c]\n", 
            pInvestorMargin->LongMarginRate, 
            pInvestorMargin->LongMarginAmt, 
            pInvestorMargin->HedgeFlag);
    }

    /// trr46:交易编码组合持仓查询应答
    void TspiSimpleHandler::OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr47:交易编码单腿持仓查询应答
    void TspiSimpleHandler::OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr48:合约组信息查询应答
    void TspiSimpleHandler::OnRspQryInstrumentGroup(CUstpFtdcRspInstrumentGroupField *pRspInstrumentGroup, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr49:组合保证金类型查询应答
    void TspiSimpleHandler::OnRspQryClientMarginCombType(CUstpFtdcRspClientMarginCombTypeField *pRspClientMarginCombType, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr50:行权录入应答
    void TspiSimpleHandler::OnRspExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr51:行权操作应答
    void TspiSimpleHandler::OnRspExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

    /// trr52:行权通知
    void TspiSimpleHandler::OnRtnExecOrder(CUstpFtdcExecOrderField *pExecOrder) {}

    /// trr53:行权录入错误回报
    void TspiSimpleHandler::OnErrRtnExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo) {}

    /// trr54:行权操作错误回报
    void TspiSimpleHandler::OnErrRtnExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo) {}

    /// trr55:主次席资金同步通知
    void TspiSimpleHandler::OnRtnTransferMoney(CUstpFtdcSyncMoneyTransferField *pSyncMoneyTransfer) {}

    /// trr56 OnRspQrySystemTime() 系统时间查询应答
    void TspiSimpleHandler::OnRspQrySystemTime(CUstpFtdcRspQrySystemTimeField *pRspQrySystemTime, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("trr56:OnRspQrySystemTime():");
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
        printf("trr56:ExchangeID=[%s],SystemTime=[%s]\n", 
            pRspQrySystemTime->ExchangeID, 
            pRspQrySystemTime->SystemTime);
        // tqq29
    }

// private:
//     // 指向CUstpFtdcMduserApi实例的指针
//     CUstpFtdcTraderApi* t_pUserApi;
// };



    // mrr01 OnFrontConnected 当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    // 当客户端与行情发布服务器建立起通信连接，客户端需要进行登录
    void MspiSimpleHandler::OnFrontConnected() {

        dnj.ttMconnect = get_currentTT();
        dnj.isMConnectOK = true;
        ttTime tt_data = dnj.ttMconnect;
        printf("%s,mrr01,OnFrontConnected()\n", dnj.ttMconnect.datentime_micro); 

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr01"                 );   
        sprintf(cFields[ 3], "%s", "OnFrontConnected()" );
        sprintf(cFields[ 4], "%s", "mrtn"                  );            
      
        send_sline_spi_output(cFields, 20);

        if (dnj.isMGoLogin) {
            //登录
            char sSource_au[10] = "au";
            char sCondition_au[10] = "if006";
            console_in_mqq01(sSource_au, sCondition_au);
        }
    }
        
    // mrr03 OnFrontDisconnected 当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    ///@param nReason 错误原因
    ///        0x1001 网络读失败     4097
    ///        0x1002 网络写失败     4098
    ///        0x2001 接收心跳超时   8193
    ///        0x2002 发送心跳失败   8194
    ///        0x2003 收到错误报文   8195
    void MspiSimpleHandler::OnFrontDisconnected(int nReason){
        
        // 当发生这个情况后，API会自动重新连接，客户端可不做处理
        dnj.ttMdisconnect = get_currentTT();
        dnj.ttMlogout = get_currentTT();
        dnj.isMConnectOK = false;
        dnj.isMLoginOK = false; //??? 不確定這裡要不要 log rr_rsp (不確定會不會自動呼叫 logout())
        ttTime tt_data = dnj.ttMdisconnect;
        printf("%s,mrr03,OnFrontDisconnected(),nReason=%d\n", dnj.ttMdisconnect.datentime_micro, nReason); 
         
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr03"                 );   
        sprintf(cFields[ 3], "%s", "OnFrontDisconnected()" );
        sprintf(cFields[ 4], "%s", "mrtn"                  );            
      
        sprintf(cFields[20],"%d", nReason);   // nReason
        
        send_sline_spi_output(cFields, 21);
    };
        
    ///mrr05 心跳超时警告。当长时间未收到报文时，该方法被调用。
    ///@param nTimeLapse 距离上次接收报文的时间
    void MspiSimpleHandler::OnHeartBeatWarning(int nTimeLapse){
        printf(">>>>>>>>>>>>>>>>>>>>,mrr05,OnHeartBeatWarning(),nTimeLapse=[%d]\n", nTimeLapse);
        // mrr05,OnHeartBeatWarning(),nTimeLapse=[6] 2019/04/18 12:49:57 268772
    }
    
    ///mrr06 报文回调开始通知。当API收到一个报文后，首先调用本方法，然后是各数据域的回调，最后是报文回调结束通知。
    ///@param nTopicID 主题代码（如私有流、公共流、行情流等）
    ///@param nSequenceNo 报文序号
    void MspiSimpleHandler::OnPackageStart(int nTopicID, int nSequenceNo){};
    
    ///mrr07 报文回调结束通知。当API收到一个报文后，首先调用报文回调开始通知，然后是各数据域的回调，最后调用本方法。
    ///@param nTopicID 主题代码（如私有流、公共流、行情流等）
    ///@param nSequenceNo 报文序号
    void MspiSimpleHandler::OnPackageEnd(int nTopicID, int nSequenceNo){};
        
        
    // mrr08 OnRspError() 错误应答。针对用户请求的出错通知 
    void MspiSimpleHandler::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,mrr08,OnRspError(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr08"                 );   
        sprintf(cFields[ 3], "%s", "OnRspError()"  );
        sprintf(cFields[ 4], "%s", "mrsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

        send_sline_spi_output(cFields, 20);
        // 客户端需进行错误处理
    }
    
    // mrr09 OnRspUserLogin 风控前置系统用户登录应答。当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    void MspiSimpleHandler::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,mrr09,OnRspUserLogin(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr09"                 );   
        sprintf(cFields[ 3], "%s", "OnRspUserLogin()"  );
        sprintf(cFields[ 4], "%s", "mrsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 

        char uTradingSystemName[200];
        memset(&uTradingSystemName, 0, sizeof(uTradingSystemName));
        my_iconv("GB2312", "UTF-8", pRspUserLogin->TradingSystemName, uTradingSystemName); // (from, to, in, out)
        
        sprintf(cFields[20],"%s", pRspUserLogin->TradingDay       ) ; ///交易日                   TUstpFtdcDateType                          
        sprintf(cFields[21],"%s", pRspUserLogin->BrokerID         ) ; ///经纪公司编号             TUstpFtdcBrokerIDType
        sprintf(cFields[22],"%s", pRspUserLogin->UserID           ) ; ///交易用户代码             TUstpFtdcUserIDType    
        sprintf(cFields[23],"%s", pRspUserLogin->LoginTime        ) ; ///登录成功时间             TUstpFtdcTimeType    
        sprintf(cFields[24],"%s", pRspUserLogin->ExchangeTime     ) ; ///登录成功时的交易所时间   TUstpFtdcTimeType    
        sprintf(cFields[25],"%s", pRspUserLogin->MaxOrderLocalID  ) ; ///用户最大本地报单号       TUstpFtdcUserOrderLocalIDType    
        sprintf(cFields[26],"%s", uTradingSystemName              ) ; ///交易系统名称             TUstpFtdcTradingSystemNameType    
        sprintf(cFields[27],"%d", pRspUserLogin->DataCenterID     ) ; ///数据中心代码             TUstpFtdcDataCenterIDType    
        sprintf(cFields[28],"%d", pRspUserLogin->PrivateFlowSize  ) ; ///会员私有流当前长度       TUstpFtdcSequenceNoType    
        sprintf(cFields[29],"%d", pRspUserLogin->UserFlowSize     ) ; ///交易员私有流当前长度     TUstpFtdcSequenceNoType   
        sprintf(cFields[30],"%s", pRspUserLogin->ActionDay        ) ; ///业务发生日期             TUstpFtdcDateType    
        sprintf(cFields[31],"%s", pRspUserLogin->FemasVersion     ) ; ///飞马版本号               TUstpFtdcFemasVersionType    
        sprintf(cFields[32],"%d", pRspUserLogin->FemasLifeCycle   ) ; ///飞马生命周期号           TUstpFtdcFemasLifeCycleType    

        send_sline_spi_output(cFields, 33);
        
        if (pRspInfo->ErrorID == 0) {
            // 登录成功
            dnj.ttMlogin = get_currentTT();
            dnj.isMLoginOK = true;
        } else {
            // 指令失败，客户端需进行错误处理 (不一定會登出, 只是不改變狀態)
            // ErrorCode=[59], ErrorMsg=ErrorMsg=[用户重复登录]
        }
            
        if (dnj.isMGoSub) {
            //订阅合
            char sSource_au[10] = "au";
            char sCondition_au[10] = "if007";
            console_in_mqq04(sSource_au, sCondition_au);
        }

    }
    
    // mrr10 OnRspUserLogout 用户退出应答
    void MspiSimpleHandler::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

        char uErrorMsg[200];
        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
        printf("%s,mrr10,OnRspUserLogout(),", tt_data.datentime_micro); 
        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr10"                 );   
        sprintf(cFields[ 3], "%s", "OnRspUserLogout()"  );
        sprintf(cFields[ 4], "%s", "mrsp"                  );            
        sprintf(cFields[ 5], "%d", pRspInfo->ErrorID       );
        sprintf(cFields[ 6], "%s", uErrorMsg               );
        sprintf(cFields[ 7], "%d", nRequestID              );
        sprintf(cFields[ 8], "%d", bIsLast                 );
      //sprintf(cFields[ 9], "%s", dnj.filenamee_rr_rsp    ); 
            
        sprintf(cFields[20],"%s", pRspUserLogout->BrokerID         ) ; ///经纪公司编号 TUstpFtdcBrokerIDType
        sprintf(cFields[21],"%s", pRspUserLogout->UserID           ) ; ///交易用户代码 TUstpFtdcUserIDType    

        send_sline_spi_output(cFields, 22);

        if (pRspInfo->ErrorID == 0) {
            // 退出成功
            dnj.ttMlogout = tt_data;
            dnj.isMLoginOK = false;
        } else {
            // 指令失败，客户端需进行错误处理 (不一定會登出, 只是不改變狀態)
            // ErrorCode=
        }
    }

    // mrr11 OnRtnDepthMarketData 深度行情通知，行情服务器会主动通知客户端
    void MspiSimpleHandler::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pMarketData) {
    
        // 客户端按需处理返回的数据
        // 遇到 double 不會 "" 而是 DBL_MAX
        // (pMarketData -> AskPrice1 == DBL_MAX) ? printf("-,"):printf("%f,", pMarketData -> AskPrice1);
     
        // rtn 的 規格, header 加上這3欄位就好了
        ttTime tt_data = get_currentTT();
        //printf("%s,mrr11,OnRtnDepthMarketData(),www,InstrumentID=[%s]\n", tt_data.datentime_micro, pMarketData->InstrumentID); //www
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr11"                 );   
        sprintf(cFields[ 3], "%s", "OnRtnDepthMarketData()");
        sprintf(cFields[ 4], "%s", "mrtn"                  );                    
        
                                                                                        sprintf(cFields[20],"%s", pMarketData ->    TradingDay          );  ///交易日          %s  20190301           TUstpFtdcDateType                
                                                                                        sprintf(cFields[21],"%s", pMarketData ->    SettlementGroupID   );  ///结算组代码      %s  SG01               TUstpFtdcSettlementGroupIDType    
                                                                                        sprintf(cFields[22],"%d", pMarketData ->    SettlementID        );  ///结算编号        %d  1                  TUstpFtdcSettlementIDType        
        (pMarketData -> PreSettlementPrice  == DBL_MAX) ? sprintf(cFields[23],"-")    : sprintf(cFields[23],"%f", pMarketData ->    PreSettlementPrice  );  ///昨结算          %f  3677.000000        TUstpFtdcPriceType                
        (pMarketData -> PreClosePrice       == DBL_MAX) ? sprintf(cFields[24],"-")    : sprintf(cFields[24],"%f", pMarketData ->    PreClosePrice       );  ///昨收盘          %f  3678.000000        TUstpFtdcPriceType                
        (pMarketData -> PreOpenInterest     == DBL_MAX) ? sprintf(cFields[25],"-")    : sprintf(cFields[25],"%f", pMarketData ->    PreOpenInterest     );  ///昨持仓量        %f  10694.000000       TUstpFtdcLargeVolumeType        
        (pMarketData -> PreDelta            == DBL_MAX) ? sprintf(cFields[26],"-")    : sprintf(cFields[26],"%f", pMarketData ->    PreDelta            );  ///昨虚实度        %f  -                  TUstpFtdcRatioType                
        (pMarketData -> OpenPrice           == DBL_MAX) ? sprintf(cFields[27],"-")    : sprintf(cFields[27],"%f", pMarketData ->    OpenPrice           );  ///今开盘          %f  3707.200000        TUstpFtdcPriceType                
        (pMarketData -> HighestPrice        == DBL_MAX) ? sprintf(cFields[28],"-")    : sprintf(cFields[28],"%f", pMarketData ->    HighestPrice        );  ///最高价          %f  3718.000000        TUstpFtdcPriceType                
        (pMarketData -> LowestPrice         == DBL_MAX) ? sprintf(cFields[29],"-")    : sprintf(cFields[29],"%f", pMarketData ->    LowestPrice         );  ///最低价          %f  3707.200000        TUstpFtdcPriceType                
        (pMarketData -> ClosePrice          == DBL_MAX) ? sprintf(cFields[30],"-")    : sprintf(cFields[30],"%f", pMarketData ->    ClosePrice          );  ///今收盘          %f  -                  TUstpFtdcPriceType                
        (pMarketData -> UpperLimitPrice     == DBL_MAX) ? sprintf(cFields[31],"-")    : sprintf(cFields[31],"%f", pMarketData ->    UpperLimitPrice     );  ///涨停板价        %f  4044.600000        TUstpFtdcPriceType                
        (pMarketData -> LowerLimitPrice     == DBL_MAX) ? sprintf(cFields[32],"-")    : sprintf(cFields[32],"%f", pMarketData ->    LowerLimitPrice     );  ///跌停板价        %f  3309.400000        TUstpFtdcPriceType                
        (pMarketData -> SettlementPrice     == DBL_MAX) ? sprintf(cFields[33],"-")    : sprintf(cFields[33],"%f", pMarketData ->    SettlementPrice     );  ///今结算          %f  -                  TUstpFtdcPriceType                
        (pMarketData -> CurrDelta           == DBL_MAX) ? sprintf(cFields[34],"-")    : sprintf(cFields[34],"%f", pMarketData ->    CurrDelta           );  ///今虚实度        %f  -                  TUstpFtdcRatioType                
        (pMarketData -> LastPrice           == DBL_MAX) ? sprintf(cFields[35],"-")    : sprintf(cFields[35],"%f", pMarketData ->    LastPrice           );  ///最新价          %f  3709.800000        TUstpFtdcPriceType                
                                                                                        sprintf(cFields[36],"%d", pMarketData ->    Volume              );  ///数量            %d  141                TUstpFtdcVolumeType                
        (pMarketData -> Turnover            == DBL_MAX) ? sprintf(cFields[37],"-")    : sprintf(cFields[37],"%f", pMarketData ->    Turnover            );  ///成交金额        %f  156965820.000000   TUstpFtdcMoneyType                
        (pMarketData -> OpenInterest        == DBL_MAX) ? sprintf(cFields[38],"-")    : sprintf(cFields[38],"%f", pMarketData ->    OpenInterest        );  ///持仓量          %f  10695.000000       TUstpFtdcLargeVolumeType        
        (pMarketData -> BidPrice1           == DBL_MAX) ? sprintf(cFields[39],"-")    : sprintf(cFields[39],"%f", pMarketData ->    BidPrice1           );  ///申买价一        %f  3709.200000        TUstpFtdcPriceType                
                                                                                        sprintf(cFields[40],"%d", pMarketData ->    BidVolume1          );  ///申买量一        %d  1                  TUstpFtdcVolumeType                
        (pMarketData -> AskPrice1           == DBL_MAX) ? sprintf(cFields[41],"-")    : sprintf(cFields[41],"%f", pMarketData ->    AskPrice1           );  ///申卖价一        %f  3710.000000        TUstpFtdcPriceType                
                                                                                        sprintf(cFields[42],"%d", pMarketData ->    AskVolume1          );  ///申卖量一        %d  5                  TUstpFtdcVolumeType                
        (pMarketData -> BidPrice2           == DBL_MAX) ? sprintf(cFields[43],"-")    : sprintf(cFields[43],"%f", pMarketData ->    BidPrice2           );  ///申买价二        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[44],"%d", pMarketData ->    BidVolume2          );  ///申买量二        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> BidPrice3           == DBL_MAX) ? sprintf(cFields[45],"-")    : sprintf(cFields[45],"%f", pMarketData ->    BidPrice3           );  ///申买价三        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[46],"%d", pMarketData ->    BidVolume3          );  ///申买量三        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> AskPrice2           == DBL_MAX) ? sprintf(cFields[47],"-")    : sprintf(cFields[47],"%f", pMarketData ->    AskPrice2           );  ///申卖价二        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[48],"%d", pMarketData ->    AskVolume2          );  ///申卖量二        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> AskPrice3           == DBL_MAX) ? sprintf(cFields[49],"-")    : sprintf(cFields[49],"%f", pMarketData ->    AskPrice3           );  ///申卖价三        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[50],"%d", pMarketData ->    AskVolume3          );  ///申卖量三        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> BidPrice4           == DBL_MAX) ? sprintf(cFields[51],"-")    : sprintf(cFields[51],"%f", pMarketData ->    BidPrice4           );  ///申买价四        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[52],"%d", pMarketData ->    BidVolume4          );  ///申买量四        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> BidPrice5           == DBL_MAX) ? sprintf(cFields[53],"-")    : sprintf(cFields[53],"%f", pMarketData ->    BidPrice5           );  ///申买价五        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[54],"%d", pMarketData ->    BidVolume5          );  ///申买量五        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> AskPrice4           == DBL_MAX) ? sprintf(cFields[55],"-")    : sprintf(cFields[55],"%f", pMarketData ->    AskPrice4           );  ///申卖价四        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[56],"%d", pMarketData ->    AskVolume4          );  ///申卖量四        %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> AskPrice5           == DBL_MAX) ? sprintf(cFields[57],"-")    : sprintf(cFields[57],"%f", pMarketData ->    AskPrice5           );  ///申卖价五        %f  -                  TUstpFtdcPriceType                
                                                                                        sprintf(cFields[58],"%d", pMarketData ->    AskVolume5          );  ///申卖量五        %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[59],"%s", pMarketData ->    InstrumentID        );  ///合约代码        %s  IF1903             TUstpFtdcInstrumentIDType        
                                                                                        sprintf(cFields[60],"%s", pMarketData ->    UpdateTime          );  ///最后修改时间    %s  09:30:30           TUstpFtdcTimeType                
                                                                                        sprintf(cFields[61],"%d", pMarketData ->    UpdateMillisec      );  ///最后修改毫秒    %d  800                TUstpFtdcMillisecType            
                                                                                        sprintf(cFields[62],"%s", pMarketData ->    ActionDay           );  ///业务发生日期    %s                     TUstpFtdcDateType                
        (pMarketData -> HisHighestPrice     == DBL_MAX) ? sprintf(cFields[63],"-")    : sprintf(cFields[63],"%f", pMarketData ->    HisHighestPrice     );  ///历史最高价      %f  0.000000           TUstpFtdcPriceType                
        (pMarketData -> HisLowestPrice      == DBL_MAX) ? sprintf(cFields[64],"-")    : sprintf(cFields[64],"%f", pMarketData ->    HisLowestPrice      );  ///历史最低价      %f  0.000000           TUstpFtdcPriceType                
                                                                                        sprintf(cFields[65],"%d", pMarketData ->    LatestVolume        );  ///最新成交量      %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[66],"%d", pMarketData ->    InitVolume          );  ///初始持仓量      %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[67],"%d", pMarketData ->    ChangeVolume        );  ///持仓量变化      %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[68],"%d", pMarketData ->    BidImplyVolume      );  ///申买推导量      %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[69],"%d", pMarketData ->    AskImplyVolume      );  ///申卖推导量      %d  0                  TUstpFtdcVolumeType                
        (pMarketData -> AvgPrice            == DBL_MAX) ? sprintf(cFields[70],"-")    : sprintf(cFields[70],"%f", pMarketData ->    AvgPrice            );  ///当日均价        %f  0.000000           TUstpFtdcPriceType                
                                                                                        sprintf(cFields[71],"%c", pMarketData ->    ArbiType            );  ///策略类别        %c                     TUstpFtdcArbiTypeType            
                                                                                        sprintf(cFields[72],"%s", pMarketData ->    InstrumentID_1      );  ///第一腿合约代码  %s                       TUstpFtdcInstrumentIDType        
                                                                                        sprintf(cFields[73],"%s", pMarketData ->    InstrumentID_2      );  ///第二腿合约代码  %s                       TUstpFtdcInstrumentIDType        
                                                                                        sprintf(cFields[74],"%s", pMarketData ->    InstrumentName      );  ///合约名称        %s                       TUstpFtdcInstrumentIDType        
                                                                                        sprintf(cFields[75],"%d", pMarketData ->    TotalBidVolume      );  ///总买入量        %d  0                  TUstpFtdcVolumeType                
                                                                                        sprintf(cFields[76],"%d", pMarketData ->    TotalAskVolume      );  ///总卖出量        %d  0                  TUstpFtdcVolumeType    
                                                                                                        
                                                                                                        
        //printf(">>%s<<\n", sline);                                                                    
        send_sline_spi_output(cFields, 77);
                
    }
        
    // mrr12 OnRspSubMarketData() 订阅合约的相关信息
    // mrr12 比較特別, nRequestID 無效 一定是  0, pRspInfo 無效 一但被呼叫就會 crash, bIsLast 有用.
    void MspiSimpleHandler::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        //printf("Sub 返回订阅合约：%s \n", pSpecificInstrument -> InstrumentID);

        // mrr12 比較特別, 
        // nRequestID 無效 一定是  0, 
        // pRspInfo 無效 一但被呼叫就會 crash, 
        // 只有 bIsLast 有用.
//        char uErrorMsg[200];
//        memset(&uErrorMsg, 0, sizeof(uErrorMsg));
//        my_iconv("GB2312", "UTF-8", pRspInfo -> ErrorMsg, uErrorMsg); // (from, to, in, out)
        ttTime tt_data = get_currentTT();
//        printf("%s,mrr10,OnRspUserLogout(),", tt_data.datentime_micro); 
//        printf("ErrorCode=[%d],ErrorMsg=[%s],RequestID=[%d],Chain=[%d]\n", pRspInfo->ErrorID, uErrorMsg, nRequestID, bIsLast);

        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr12"                 );   
        sprintf(cFields[ 3], "%s", "OnRspSubMarketData()"  );
        sprintf(cFields[ 4], "%s", "mrtn"                  );   
      
        sprintf(cFields[20],"%s", pSpecificInstrument -> InstrumentID) ; ///合约代码 TUstpFtdcInstrumentIDType
        sprintf(cFields[21],"%d", bIsLast) ;
        
        send_sline_spi_output(cFields, 22);
    }
        
    // mrr13 OnRspUnSubMarketData() 订阅合约的相关信息
    // mrr13 目前沒有作用
    // mrr13 比較特別, nRequestID 無效 一定是  0, pRspInfo 無效 一但被呼叫就會 crash, bIsLast 有用.
    void MspiSimpleHandler::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
//        printf("UnSub 返回订阅合约：%s \n", pSpecificInstrument -> InstrumentID);
        ttTime tt_data = get_currentTT();
        
        char *cFields[100];
        for (int idx=0; idx<100; idx++) cFields[idx] = new char[200];
        for (int idx=0; idx<100; idx++) strcpy(cFields[idx], "");
        sprintf(cFields[ 0], "%s", tt_data.datentime_micro );
        sprintf(cFields[ 1], "%s", tt_data.timestamp_micro );
        sprintf(cFields[ 2], "%s", "mrr13"                 );   
        sprintf(cFields[ 3], "%s", "OnRspUnSubMarketData()"  );
        sprintf(cFields[ 4], "%s", "mrtn"                  );   
      
        sprintf(cFields[20],"%s", pSpecificInstrument -> InstrumentID) ; ///合约代码 TUstpFtdcInstrumentIDType
        sprintf(cFields[21],"%d", bIsLast) ;
        
        send_sline_spi_output(cFields, 22);

    }
        
    ///mrr14 获取行情主题信息应答
    void MspiSimpleHandler::OnRspGetMarketTopic(CUstpFtdcRspMarketTopicField *pRspMarketTopic, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///mrr15 获取行情快照应答
    void MspiSimpleHandler::OnRspGetMarketData(CUstpFtdcRspDepthMarketDataField *pRspDepthMarketData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

//     private:
//         // 指向CUstpFtdcMduserApi实例的指针
//         CUstpFtdcMduserApi *m_pUserApi;
// };

/*

rsp done (有 ErrorMsg, RequestID, bIsLast, data):
*trr08 OnRspError() 针对用户请求的出错通知 (這個比較特別, 只有 pRspInfo, nRequestID, bIsLast)
*trr09 OnRspUserLogin() 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
*trr10 OnRspUserLogout() 用户退出应答
*trr12 OnRspOrderInsert() 报单录入应答
*trr13 OnRspOrderAction() 报单操作应答
*trr35 OnRspQryOrder() 报单查询应答
*trr36 OnRspQryTrade() 成交单查询应答
*trr37 OnRspQryUserInvestor() 投资者资金账户查询请求应答
*trr38 OnRspQryTradingCode() 交易编码查询应答
*trr39 OnRspQryInvestorAccount() 投资者资金账户查询应答
*trr40 OnRspQryInstrument() 合约查询应答
*trr42 OnRspQryInvestorPosition() 投资者持仓查询应答
*mrr08 OnRspError() 错误应答。针对用户请求的出错通知 
*mrr09 OnRspUserLogin() 风控前置系统用户登录应答。当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
*mrr10 OnRspUserLogout() 用户退出应答
(mrr08 trr08 crr08 沒有 data)


err done (有 ErrorMsg, 有 data,  沒有 RequestID, bIsLast):
*trr23 OnErrRtnOrderInsert() 报单录入错误回报
*trr24 OnErrRtnOrderAction() 

rtn done (只有 data):
*mrr11 OnRtnDepthMarketData() 深度行情通知，行情服务器会主动通知客户端 www
*trr21 OnRtnTrade() 成交回报
*trr22 OnRtnOrder() 报单回报
*trr25 OnRtnInstrumentStatus()  合约交易状态通知 www
*trr30 OnRtnForQuote() 询价回报 www (這個單純 test)
*trr01 OnFrontConnected() 当客户端与飞马平台建立起通信连接，客户端需要进行登录
*trr03 OnFrontDisconnected() 当客户端与飞马平台通信连接断开时，该方法被调用
*mrr01 OnFrontConnected() 当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
*mrr03 OnFrontDisconnected() 当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
*mrr12 OnRspSubMarketData() 订阅合约的相关信息
*mrr13 OnRspUnSubMarketData() 订阅合约的相关信息

mrr01, trr01 是 rtn
mrr03, trr03 是 rtn
EMPTY 目前有: trr35, trr36, trr40, trr42
但是理論上 Qry 的都有可能遇到 EMPTY
 
*/











