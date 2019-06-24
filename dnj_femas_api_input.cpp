/*


*/

#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // strcmp
#include <string>
#include <sys/time.h> // gettimeofday

#include <dnj_femas_common.hpp>
#include <dnj_femas_api_input.hpp>


extern dnj_femas_global_setting dnj;

void do_api_in(char* cFields[]) {
    CUstpFtdcTraderApi *pTUserApi = dnj.pTUserApi;
    CUstpFtdcMduserApi *pMUserApi = dnj.pMUserApi;
    do_api_in(cFields, pTUserApi, pMUserApi);
}

void do_api_in(char* cFields[], CUstpFtdcTraderApi* pTUserApi, CUstpFtdcMduserApi *pMUserApi) {

    int iRequestID = 0;
	int iRR = 0;

    // time
    bool isCmdTimeOK = true;

    std::string date_pst(dnj.ttpReadintxt.datentime_micro); 
    std::string date_cmd(cFields[0]);
    if (date_cmd < date_pst) {
//        printf(">>>>cmd date no ok! date_cmd:%s < date_pst:%s\n", cFields[0], dnj.ttpReadintxt.datentime_micro);
        isCmdTimeOK = false;
    } else {
//        printf(">>>>cmd date OK! date_cmd:%s >= date_pst:%s\n", cFields[0], dnj.ttpReadintxt.datentime_micro);
    }
    
    // start tqq (check cmd time or and other check cmd format...)
    if (! isCmdTimeOK) {
        return;
    }

    
//// T api command
    
    /*
    
    ///tqq000: SetHeartbeatTimeout() 设置心跳超时时间。
	///@param timeout 心跳超时时间(秒)  
	//virtual void SetHeartbeatTimeout(unsigned int timeout) = 0;

    ///tqq001: 打开请求日志文件
	///@param pszReqLogFileName 请求日志文件名  
	///@return 0 操作成功
	///@return -1 打开日志文件失败
	//virtual int OpenRequestLog(const char *pszReqLogFileName) = 0;

    ///tqq002: 打开应答日志文件
	///@param pszRspLogFileName 应答日志文件名  
	///@return 0 操作成功
	///@return -1 打开日志文件失败
	//virtual int OpenResponseLog(const char *pszRspLogFileName) = 0;

    ///tqq003: 获取当前交易日
	///@retrun 获取到的交易日 //  return "20190319"
	///@remark 只有登录成功后,才能得到正确的交易日 (??沒有連線/登入, 也有數值)
    //virtual const char *GetTradingDay() = 0;
    	
    ///tqq004: 获取系统版本号
	///@param nMajorVersion 主版本号
	///@param nMinorVersion 子版本号
	///@return 系统标识字符串
    //static const char *GetVersion(int &nMajorVersion, int &nMinorVersion);
    
	///tqq005: RegisterNameServer() 注册名字服务器网络地址
	///@param pszNsAddress：名字服务器网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
	///@remark RegisterFront优先于RegisterNameServer
	//virtual void RegisterNameServer(char *pszNsAddress) = 0;

	///tqq006: RegisterCertificateFile() 加载证书, 未啟用
	///@param pszCertFileName 用户证书文件名
	///@param pszKeyFileName 用户私钥文件名
	///@param pszCaFileName 可信任CA证书文件名
	///@param pszKeyFilePassword 用户私钥文件密码
	///@return 0 操作成功
	///@return -1 可信任CA证书载入失败
	///@return -2 用户证书载入失败
	///@return -3 用户私钥载入失败	
	///@return -4 用户证书校验失败
	//virtual int RegisterCertificateFile(const char *pszCertFileName, const char *pszKeyFileName, 
	//	const char *pszCaFileName, const char *pszKeyFilePassword) = 0;


    // tqq007: 订阅
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
    
    
    
    //tqq00: RegisterFront() 设置飞马平台服务的地址，可以注册多个地址备用
	pTUserApi -> RegisterFront(dnj.gTAddr);
	pTUserApi -> RegisterQryFront(dnj.gTAddr);
    
	///tqq01: ReqUserLogin() 风控前置系统用户登录请求
	virtual int ReqUserLogin(CUstpFtdcReqUserLoginField *pReqUserLogin, int nRequestID) = 0;
    
    ///tqq02:ReqUserLogout() 用户退出请求
    CUstpFtdcReqUserLogoutField reqLogout;
    iRequestID = get_nRequestID();
    iRR = pTUserApi->ReqUserLogout(&reqLogout, iRequestID);
    printf("qq02:ReqUserLogout():iRR=%d,nRequestID=%d\n", iRR, iRequestID);
    
    ///tqq03:ReqUserPasswordUpdate() 用户密码修改请求
    CUstpFtdcUserPasswordUpdateField reqPwdUpdate;
    iRequestID = get_nRequestID();
    iRR = pTUserApi->ReqUserPasswordUpdate(&reqPwdUpdate, iRequestID);
    printf("qq03:ReqUserPasswordUpdate():iRR=%d,nRequestID=%d\n", iRR, iRequestID);
    */

    // tqq04 ReqOrderInsert() 发出报单录入请求
    // expect: trr12 OnRspOrderInsert 报单录入应答
    // expect: trr22 OnRtnOrder() 报单回报 // much info  
    // expect: trr21 OnRtnTrade() 成交回报
    // expect: trr23 OnErrRtnOrderInsert() 报单录入错误回报
    if (strcmp(cFields[2], "tqq04") == 0) {
        CUstpFtdcInputOrderField qryordr;
        memset(&qryordr, 0, sizeof(qryordr));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryordr.BrokerID            ,           cFields[20]    ); // "0059"     ///经纪公司编号,必填字段                                                      TUstpFtdcBrokerIDType 
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryordr.ExchangeID          ,           cFields[21]    ); // "CFFEX"    ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）                  TUstpFtdcExchangeIDType 
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryordr.OrderSysID          ,           cFields[22]    ); // ""         ///系统报单编号                                                               TUstpFtdcOrderSysIDType 
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qryordr.InvestorID          ,           cFields[23]    ); // "990407"   ///投资者编号,必填字段 （客户资金账户）                                       TUstpFtdcInvestorIDType 
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qryordr.UserID              ,           cFields[24]    ); // "990407"   //用户代码,必填字段（飞马用户代码）                                           TUstpFtdcUserIDType 
        if (strcmp(cFields[25], "~X~") != 0)        qryordr.SeatNo              =  atoi(    cFields[25]    ); // ""         ///指定下单席位编号（取值范围[1-N]，N为可用席位数目，超出范围的随机分配席位） TUstpFtdcSeatNoType 
        if (strcmp(cFields[26], "~X~") != 0) strcpy(qryordr.InstrumentID        ,           cFields[26]    ); // "IF1904"   ///合约代码,必填字段                                                          TUstpFtdcInstrumentIDType     
        if (strcmp(cFields[27], "~X~") != 0) strcpy(qryordr.UserOrderLocalID    ,           cFields[27]    ); // "00001"    ///用户本地报单号,必填字段                                                    TUstpFtdcUserOrderLocalIDType 
        if (strcmp(cFields[28], "~X~") != 0)        qryordr.OrderPriceType      =  (char)   cFields[28][0]  ; // '3'        ///报单价格条件,必填字段（支持市价、限价、五档价和最优价）                    TUstpFtdcOrderPriceTypeType 
        if (strcmp(cFields[29], "~X~") != 0)        qryordr.Direction           =  (char)   cFields[29][0]  ; // '0'        ///买卖方向,必填字段                                                          TUstpFtdcDirectionType 
        if (strcmp(cFields[30], "~X~") != 0)        qryordr.OffsetFlag          =  (char)   cFields[30][0]  ; // '0'        ///开平标志,必填字段（中金所支持开仓和平仓，上期所支持开仓、平今和平昨）      TUstpFtdcOffsetFlagType 
        if (strcmp(cFields[31], "~X~") != 0)        qryordr.HedgeFlag           =  (char)   cFields[31][0]  ; // '1'        ///投机套保标志,必填字段（支持投机、套保、套利、做市商编码）                  TUstpFtdcHedgeFlagType 
        if (strcmp(cFields[32], "~X~") != 0)        qryordr.LimitPrice          =  atof(    cFields[32]    ); // "500"      ///价格,必填字段                                                              TUstpFtdcLimitPriceType 
        if (strcmp(cFields[33], "~X~") != 0)        qryordr.Volume              =  atoi(    cFields[33]    ); // '1'        ///数量,必填字段                                                              TUstpFtdcVolumeType 
        if (strcmp(cFields[34], "~X~") != 0)        qryordr.TimeCondition       =  (char)   cFields[34][0]  ; // '3'        ///有效期类型,必填字段（“立即完成，否则撤销”1 OR“当日有效”3）                 TUstpFtdcTimeConditionType 
        if (strcmp(cFields[35], "~X~") != 0) strcpy(qryordr.GTDDate             ,           cFields[35]    ); // ""         ///GTD日期（暂不支持，保留域）                                                TUstpFtdcDateType 
        if (strcmp(cFields[36], "~X~") != 0)        qryordr.VolumeCondition     =  (char)   cFields[36][0]  ; // '1'        ///成交量类型,必填字段（支持“任意数量”和“全部数量”）                          TUstpFtdcVolumeConditionType 
        if (strcmp(cFields[37], "~X~") != 0)        qryordr.MinVolume           =  atoi(    cFields[37]    ); // ""         ///最小成交量（暂不支持，保留域）                                             TUstpFtdcVolumeType 
        if (strcmp(cFields[38], "~X~") != 0)        qryordr.StopPrice           =  atof(    cFields[38]    ); // ""         ///止损价（暂不支持，保留域）                                                 TUstpFtdcPriceType 
        if (strcmp(cFields[39], "~X~") != 0)        qryordr.ForceCloseReason    =  (char)   cFields[39][0]  ; // '0'        ///强平原因，只支持”非强平” ? （暂不支持，保留域）                            TUstpFtdcForceCloseReasonType 
        if (strcmp(cFields[40], "~X~") != 0)        qryordr.IsAutoSuspend       =  atoi(    cFields[40]    ); // ""         ///自动挂起标志（暂不支持，保留域）                                           TUstpFtdcBoolType 
        if (strcmp(cFields[41], "~X~") != 0) strcpy(qryordr.BusinessUnit        ,           cFields[41]    ); // ""         ///业务单元（暂不支持，保留域）                                               TUstpFtdcBusinessUnitType 
        if (strcmp(cFields[42], "~X~") != 0) strcpy(qryordr.UserCustom          ,           cFields[42]    ); // "self"     ///用户自定义域 64字节                                                        TUstpFtdcCustomType 
        if (strcmp(cFields[43], "~X~") != 0)        qryordr.BusinessLocalID     =  atoi(    cFields[43]    ); // ""         ///本地业务标识（暂不支持，保留域）                                           TUstpFtdcBusinessLocalIDType 
        if (strcmp(cFields[44], "~X~") != 0) strcpy(qryordr.ActionDay           ,           cFields[44]    ); // ""         ///业务发生日期                                                               TUstpFtdcDateType 
        if (strcmp(cFields[45], "~X~") != 0)        qryordr.ArbiType            =  (char)   cFields[45][0]  ; // ""         ///策略类别（套利组合业务必填字段）                                           TUstpFtdcArbiTypeType 
        if (strcmp(cFields[46], "~X~") != 0) strcpy(qryordr.ClientID            ,           cFields[46]    ); // "01568909" ///客户编码                                                                   TUstpFtdcClientIDType 
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqOrderInsert(&qryordr, iRequestID); // 发出 报单录入请求
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 47);

        printf("%s,tqq04,ReqOrderInsert(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);	
    
    } // end of if (strcmp(cFields[2], "tqq04") == 0) {
    
    // tqq05 ReqOrderAction() 报单操作请求
    // expect: trr13 OnRspOrderAction() 报单操作应答
    if (strcmp(cFields[2], "tqq05") == 0) {
        CUstpFtdcOrderActionField ordract;
        memset(&ordract, 0, sizeof(ordract));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(ordract.ExchangeID              ,           cFields[20]    ); ///交易所代码,必填字段 TUstpFtdcExchangeIDType
        if (strcmp(cFields[21], "~X~") != 0) strcpy(ordract.OrderSysID              ,           cFields[21]    ); ///交易所系统报单编号（不为空按该字段操作，为空按本地报单编号操作） TUstpFtdcOrderSysIDType 
        if (strcmp(cFields[22], "~X~") != 0) strcpy(ordract.BrokerID                ,           cFields[22]    ); ///经纪公司编号,必填字段 TUstpFtdcBrokerIDType
        if (strcmp(cFields[23], "~X~") != 0) strcpy(ordract.InvestorID              ,           cFields[23]    ); ///投资者编号,必填字段 TUstpFtdcInvestorIDType
        if (strcmp(cFields[24], "~X~") != 0) strcpy(ordract.UserID                  ,           cFields[24]    ); ///用户代码,必填字段 TUstpFtdcUserIDType
        if (strcmp(cFields[25], "~X~") != 0) strcpy(ordract.UserOrderActionLocalID  ,           cFields[25]    ); ///本次撤单Req的本地编号，必填字段 （需要保证按字典序递增） TUstpFtdcUserOrderLocalIDType
        if (strcmp(cFields[26], "~X~") != 0) strcpy(ordract.UserOrderLocalID        ,           cFields[26]    ); ///被撤订单的本地报单编号 TUstpFtdcUserOrderLocalIDType
        if (strcmp(cFields[27], "~X~") != 0)        ordract.ActionFlag              =  (char)   cFields[27][0]  ; ///报单操作标志，必填字段（只支持删除） TUstpFtdcActionFlagType
        if (strcmp(cFields[28], "~X~") != 0)        ordract.LimitPrice              =  atof(    cFields[28]    ); ///价格 （暂不支持，保留域） TUstpFtdcPriceType
        if (strcmp(cFields[29], "~X~") != 0)        ordract.VolumeChange            =  atoi(    cFields[29]    ); ///数量变化（暂不支持，保留域） TUstpFtdcVolumeType
        if (strcmp(cFields[30], "~X~") != 0)        ordract.BusinessLocalID         =  atoi(    cFields[30]    ); ///本地业务标识 TUstpFtdcBusinessLocalIDType
        if (strcmp(cFields[31], "~X~") != 0) strcpy(ordract.ClientID                ,           cFields[31]    ); ///客户编码 TUstpFtdcClientIDType
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqOrderAction(&ordract, iRequestID); // 发出 报单操作请求
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 32);

        printf("%s,tqq05,ReqOrderAction(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);	
    } // end of if (strcmp(cFields[2], "tqq05") == 0) {
    
    /*
    // tqq06:请求报价录入
    virtual int ReqQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, int nRequestID) = 0;
    // tqq07:报价操作请求
    virtual int ReqQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, int nRequestID) = 0;
    // tqq08:客户询价请求
    virtual int ReqForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, int nRequestID) = 0;
    // tqq09:客户申请组合请求
    virtual int ReqMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, int nRequestID) = 0;
    // tqq10:用户请求出入金
    virtual int ReqUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, int nRequestID) = 0;
    // tqq11:用户主次席出入金请求
    virtual int ReqTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, int nRequestID) = 0;
    */
    
    // tqq12 ReqQryOrder() 报单查询请求
    // expect: trr35 OnRspQryOrder() 报单查询应答
    if (strcmp(cFields[2], "tqq12") == 0) {
        CUstpFtdcQryOrderField qryordr;
        memset(&qryordr, 0, sizeof(qryordr));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryordr.BrokerID            ,           cFields[20]    ); ///经纪公司编号     TUstpFtdcBrokerIDType   
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryordr.UserID              ,           cFields[21]    ); ///用户代码     TUstpFtdcUserIDType         
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryordr.ExchangeID          ,           cFields[22]    ); ///交易所代码     TUstpFtdcExchangeIDType   
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qryordr.InvestorID          ,           cFields[23]    ); ///投资者编号     TUstpFtdcInvestorIDType   
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qryordr.OrderSysID          ,           cFields[24]    ); ///系统报单编号     TUstpFtdcOrderSysIDType 
        if (strcmp(cFields[25], "~X~") != 0) strcpy(qryordr.InstrumentID        ,           cFields[25]    ); ///合约代码     TUstpFtdcInstrumentIDType   
        if (strcmp(cFields[26], "~X~") != 0)        qryordr.OrderStatus         =  (char)   cFields[26][0]  ; ///报单状态     TUstpFtdcOrderStatusType    
        if (strcmp(cFields[27], "~X~") != 0)        qryordr.OrderType           =  (char)   cFields[27][0]  ; ///委托类型     TUstpFtdcOrderTypeType      
        if (strcmp(cFields[28], "~X~") != 0) strcpy(qryordr.ClientID            ,           cFields[28]    ); ///客户编码     TUstpFtdcClientIDType       
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryOrder(&qryordr, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 29);

        printf("%s,tqq12,ReqQryOrder(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq12") == 0) {
    

    // tqq13 ReqQryTrade() 成交单查询请求
    // expect: trr36 OnRspQryTrade() 成交单查询应答
    if (strcmp(cFields[2], "tqq13") == 0) {
        CUstpFtdcQryTradeField qrytrde;
        memset(&qrytrde, 0, sizeof(qrytrde));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qrytrde.BrokerID        ,           cFields[20]    ); ///经纪公司编号 TUstpFtdcBrokerIDType
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qrytrde.UserID          ,           cFields[21]    ); ///用户代码 TUstpFtdcUserIDType	     
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qrytrde.ExchangeID      ,           cFields[22]    ); ///交易所代码 TUstpFtdcExchangeIDType
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qrytrde.InvestorID      ,           cFields[23]    ); ///投资者编号 TUstpFtdcInvestorIDType
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qrytrde.TradeID         ,           cFields[24]    ); ///成交编号 TUstpFtdcTradeIDType	 
        if (strcmp(cFields[25], "~X~") != 0) strcpy(qrytrde.InstrumentID    ,           cFields[25]    ); ///合约代码 TUstpFtdcInstrumentIDType
        if (strcmp(cFields[26], "~X~") != 0) strcpy(qrytrde.ClientID        ,           cFields[26]    ); ///客户编码 TUstpFtdcClientIDType	 
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryTrade(&qrytrde, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 27);

        printf("%s,tqq13,ReqQryTrade(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq13") == 0) {

    // tqq14 ReqQryUserInvestor() 可用投资者账户查询请求
    // expect: trr37:OnRspQryUserInvestor() 可用投资者账户查询应答
    if (strcmp(cFields[2], "tqq14") == 0) {
        CUstpFtdcQryUserInvestorField qryivid;
        memset(&qryivid, 0, sizeof(qryivid));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryivid.BrokerID   ,  cFields[20]    ); // "0059"     ///经纪公司编号,必填字段 TUstpFtdcBrokerIDType 
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryivid.UserID     ,  cFields[21]    ); // "990407"   //用户代码,必填字段（飞马用户代码） TUstpFtdcUserIDType 
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryUserInvestor(&qryivid, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 22);

        printf("%s,tqq14,ReqQryUserInvestor(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq14") == 0) {

    // tqq15 ReqQryTradingCode() 交易编码查询请求
    // expect: trr38:OnRspQryTradingCode 交易编码查询应答
    if (strcmp(cFields[2], "tqq15") == 0) {
        CUstpFtdcQryTradingCodeField qrytrco;
        memset(&qrytrco, 0, sizeof(qrytrco));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qrytrco.BrokerID   ,  cFields[20] );
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qrytrco.UserID     ,  cFields[21] );
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qrytrco.InvestorID ,  cFields[22] );
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qrytrco.ExchangeID ,  cFields[23] );
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qrytrco.ClientID   ,  cFields[24] ); ///客户编码
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryTradingCode(&qrytrco, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 25);

        printf("%s,tqq15,ReqQryTradingCode(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    }
    
    // tqq16 ReqQryInvestorAccount() 投资者资金账户查询请求
    // expect: trr39:OnRspQryInvestorAccount 投资者资金账户查询应答
    if (strcmp(cFields[2], "tqq16") == 0) {
        CUstpFtdcQryInvestorAccountField qryivat;
        memset(&qryivat, 0, sizeof(qryivat));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryivat.BrokerID   ,  cFields[20] );
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryivat.UserID     ,  cFields[21] );
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryivat.InvestorID ,  cFields[22] );
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryInvestorAccount(&qryivat, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 23);

        printf("%s,tqq16,ReqQryInvestorAccount(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    }
    
    // tqq17:ReqQryInstrument() 合约查询请求
    // expect: OnRspQryInstrument 合约查询应答
    if (strcmp(cFields[2], "tqq17") == 0) {
        CUstpFtdcQryInstrumentField qryinst;
        memset(&qryinst, 0, sizeof(qryinst));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryinst.ExchangeID          ,           cFields[20]    ); // "CFFEX"    ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）TUstpFtdcExchangeIDType 
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryinst.ProductID           ,           cFields[21]    ); // "??"       ///产品代码 TUstpFtdcProductIDType
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryinst.InstrumentID        ,           cFields[22]    ); // "IF1904"   ///合约代码 TUstpFtdcInstrumentIDType     
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryInstrument(&qryinst, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 23);
        printf("%s,tqq17,ReqQryInstrument(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq17") == 0) {
    
    // tqq18:ReqQryExchange() 交易所查询请求
    //virtual int ReqQryExchange(CUstpFtdcQryExchangeField *pQryExchange, int nRequestID) = 0;

    // tqq19:ReqQryInvestorPosition() 投资者持仓查询请求
    // expect: trr42 OnRspQryInvestorPosition 投资者持仓查询应答   
    if (strcmp(cFields[2], "tqq19") == 0) {
        CUstpFtdcQryInvestorPositionField qryposi;
        memset(&qryposi, 0, sizeof(qryposi));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryposi.BrokerID            ,           cFields[20]    ); // "0059"     ///经纪公司编号,必填字段 TUstpFtdcBrokerIDType 
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryposi.UserID              ,           cFields[21]    ); // "990407"   //用户代码,必填字段（飞马用户代码） TUstpFtdcUserIDType 
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryposi.ExchangeID          ,           cFields[22]    ); // "CFFEX"    ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）TUstpFtdcExchangeIDType 
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qryposi.InvestorID          ,           cFields[23]    ); // "990407"   ///投资者编号,必填字段 （客户资金账户） TUstpFtdcInvestorIDType 
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qryposi.InstrumentID        ,           cFields[24]    ); // "IF1904"   ///合约代码,必填字段 TUstpFtdcInstrumentIDType     
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryInvestorPosition(&qryposi, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 25);

        printf("%s,tqq19,ReqQryInvestorPosition(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq19") == 0) {
    
    
    // tqq20:ReqQryComplianceParam() 合规参数查询请求
    //virtual int ReqQryComplianceParam(CUstpFtdcQryComplianceParamField *pQryComplianceParam, int nRequestID) = 0;
    
    // tqq21:ReqQryInvestorFee() 投资者手续费率查询请求
    //virtual int ReqQryInvestorFee(CUstpFtdcQryInvestorFeeField *pQryInvestorFee, int nRequestID) = 0;

    // tqq22:ReqQryInvestorMargin() 投资者保证金率查询请求
    if (strcmp(cFields[2], "tqq22") == 0) {
        CUstpFtdcQryInvestorMarginField qryimgn;
        memset(&qryimgn, 0, sizeof(qryimgn));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qryimgn.BrokerID            ,           cFields[20]    ); ///经纪公司编号
        if (strcmp(cFields[21], "~X~") != 0) strcpy(qryimgn.UserID              ,           cFields[21]    ); ///用户代码
        if (strcmp(cFields[22], "~X~") != 0) strcpy(qryimgn.InvestorID          ,           cFields[22]    ); ///投资者编号
        if (strcmp(cFields[23], "~X~") != 0) strcpy(qryimgn.ExchangeID          ,           cFields[23]    ); ///交易所代码
        if (strcmp(cFields[24], "~X~") != 0) strcpy(qryimgn.InstrumentID        ,           cFields[24]    ); ///合约代码
        if (strcmp(cFields[25], "~X~") != 0) strcpy(qryimgn.ClientID            ,           cFields[25]    ); ///客户编码
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQryInvestorMargin(&qryimgn, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 26);

        printf("%s,tqq22,ReqQryInvestorMargin(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq22") == 0) {
        
    /*
    // tqq23:交易编码组合持仓查询请求
    virtual int ReqQryInvestorCombPosition(CUstpFtdcQryInvestorCombPositionField *pQryInvestorCombPosition, int nRequestID) = 0;
    
    // tqq24:交易编码单腿持仓查询请求
    virtual int ReqQryInvestorLegPosition(CUstpFtdcQryInvestorLegPositionField *pQryInvestorLegPosition, int nRequestID) = 0;
    
    // tqq25:查询合约组信息
    virtual int ReqQryInstrumentGroup(CUstpFtdcQryUstpInstrumentGroupField *pQryUstpInstrumentGroup, int nRequestID) = 0;
    
    // tqq26:查询组合保证金类型
    virtual int ReqQryClientMarginCombType(CUstpFtdcQryClientMarginCombTypeField *pQryClientMarginCombType, int nRequestID) = 0;
    
    // tqq27:行权录入请求
    virtual int ReqExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, int nRequestID) = 0;
    
    // tqq28:行权操作请求
    virtual int ReqExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID) = 0;
    */

    // tqq29 ReqQrySystemTime() 查询系统时间
    // expect: OnRspQrySystemTime rr56:系统时间查询应答
    if (strcmp(cFields[2], "tqq29") == 0) {
        CUstpFtdcReqQrySystemTimeField qrysyst;
        memset(&qrysyst, 0, sizeof(qrysyst));
        if (strcmp(cFields[20], "~X~") != 0) strcpy(qrysyst.ExchangeID          ,           cFields[20]    ); // "CFFEX"    ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）TUstpFtdcExchangeIDType 
        //iRequestID = get_nRequestID();
        iRequestID = atoi(cFields[7]);
        ttTime tt_j_time = get_currentTT();
        strcpy(cFields[15], tt_j_time.timestamp_micro);
        iRR = pTUserApi->ReqQrySystemTime(&qrysyst, iRequestID);
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields[8],"%d", iRR);
        strcpy(cFields[16], tt_f_time.timestamp_micro);
        // send
        send_sline_in_result(cFields, 21);

        printf("%s,tqq29,ReqQrySystemTime(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields[0], cFields[5], cFields[6], iRR, iRequestID);
    } // end of if (strcmp(cFields[2], "tqq29") == 0) {

    
    
//// M api command

	///mqq000: SetHeartbeatTimeout() 设置心跳超时时间。
	///@param timeout 心跳超时时间(秒)  
    //virtual void SetHeartbeatTimeout(unsigned int timeout) = 0;

    /// qq001: 打开请求日志文件 (m 沒有)
	///mqq001: SetUseMultiChannel() 是否使用多播方式接收行情
	/// @param bUseMulti 默认为false，表示不使用多播接收行情
	/// @remark 如果使用多播，则无需登录，也使用不了需要登录才能使用的接口
    //virtual void SetUseMultiChannel(bool bUseMulti = false) = 0;

    /// qq002: 打开应答日志文件 (m 沒有)
	///mqq002: RegisterMultiChannel() 注册多播通道
	///@param pMultiChannel 多播通道地址地址，
	///@remark 网络地址的例子：”multi://172.25.125@232.0.0.1:5131”。 
	///@remark “multi”代表使用多播传输；“172.25.125”用来确定接收多播的网卡；”232.0.0.1:5131”代表多播通道及端口。
	///@remark 可以多次调用，注册多个通道，接收多个通道中速度最优的行情
    //virtual void RegisterMultiChannel(const char *pMultiChannel) = 0;

	///mqq003: GetTradingDay() 获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日 (??沒有連線/登入, 也有數值)
    //virtual const char *GetTradingDay() = 0;
    
	///mqq004: GetVersion() 获取系统版本号
	///@param nMajorVersion 主版本号
	///@param nMinorVersion 子版本号
	///@return 系统标识字符串
    //static const char *GetVersion(int &nMajorVersion, int &nMinorVersion);

	///mqq005: RegisterNameServer() 注册名字服务器网络地址
	///@param pszNsAddress：名字服务器网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
	///@remark RegisterFront优先于RegisterNameServer
    //virtual void RegisterNameServer(char *pszNsAddress) = 0;
	
	///mqq006: RegisterCertificateFile() 加载证书
	///@param pszCertFileName 用户证书文件名
	///@param pszKeyFileName 用户私钥文件名
	///@param pszCaFileName 可信任CA证书文件名
	///@param pszKeyFilePassword 用户私钥文件密码
	///@return 0 操作成功
	///@return -1 可信任CA证书载入失败
	///@return -2 用户证书载入失败
	///@return -3 用户私钥载入失败	
	///@return -4 用户证书校验失败
    //virtual int RegisterCertificateFile(const char *pszCertFileName, const char *pszKeyFileName, 
    //    const char *pszCaFileName, const char *pszKeyFilePassword) = 0;


    ///mqq00: RegisterFront() 设置行情发布服务器的地址 注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	//virtual void RegisterFront(char *pszFrontAddress) = 0;
    
	///mqq01: ReqUserLogin() 风控前置系统用户登录请求
    //virtual int ReqUserLogin(CUstpFtdcReqUserLoginField *pReqUserLogin, int nRequestID) = 0;

	///mqq02: ReqUserLogout() 用户退出请求
    //virtual int ReqUserLogout(CUstpFtdcReqUserLogoutField *pReqUserLogout, int nRequestID) = 0;
    

	///mqq03: 订阅市场行情。
	///@param nTopicID 市场行情主题  
	///@param nResumeType 市场行情重传方式  
	///        USTP_TERT_RESTART:从本交易日开始重传
	///        USTP_TERT_RESUME:从上次收到的续传(非订阅全部合约时，不支持续传模式)
	///        USTP_TERT_QUICK:先传送当前行情快照,再传送登录后市场行情的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	//pMUserApi-> SubscribeMarketDataTopic (100, USTP_TERT_QUICK); //j 
    //	pMUserApi -> SubscribeMarketDataTopic(500, USTP_TERT_RESTART);
    //	pMUserApi -> SubscribeMarketDataTopic(301, USTP_TERT_RESTART);
    

	///mqq04: 订阅合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
    //virtual int SubMarketData(char *ppInstrumentID[], int nCount)=0;

	///mqq05: 退订合约行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
    //virtual int UnSubMarketData(char *ppInstrumentID[], int nCount)=0;

	///mqq06: 获取行情订阅号。
	///@param *ppExchangeID 合约ID  
	///@param nCount 要获取行情订阅号的交易所个数
	///@remark 
    //virtual int GetMarketTopic(char *pExchangeID)=0;
	
	///mqq07: 获取行情快照。
	///@param *ppExchangeID 交易所ID  
	///@param ppInstrumentID 合约ID  
	///@param nCount 要获取行情订阅号的交易所个数
	///@remark 
    //virtual int GetMarketData(char *pExchangeID, char *pInstrumentID) = 0;
	

} // function read_exe_cmd

/*

tqq04 ReqOrderInsert() 发出报单录入请求
tqq05 ReqOrderAction() 报单操作请求
tqq12 ReqQryOrder() 报单查询请求
tqq13 ReqQryTrade() 成交单查询请求
tqq14 ReqQryUserInvestor() 可用投资者账户查询请求
tqq15 ReqQryTradingCode() 交易编码查询请求
tqq16 ReqQryInvestorAccount() 投资者资金账户查询请求
tqq17:ReqQryInstrument() 合约查询请求
tqq19:ReqQryInvestorPosition() 投资者持仓查询请求
tqq22:ReqQryInvestorMargin() 投资者保证金率查询请求
tqq29 ReqQrySystemTime() 查询系统时间




*/
