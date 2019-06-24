/*


*/
#include "USTPFtdcTraderApi.h"
#include "USTPFtdcMduserApi.h"

// T
class TspiSimpleHandler : public CUstpFtdcTraderSpi {
public:
    TspiSimpleHandler(CUstpFtdcTraderApi* pUserApi) : t_pUserApi(pUserApi){};
    ~TspiSimpleHandler(){};
    void OnFrontConnected();
    void OnQryFrontConnected();
    void OnFrontDisconnected(int nReason);
    void OnQryFrontDisconnected(int nReason);
    void OnHeartBeatWarning(int nTimeLapse);
    void OnPackageStart(int nTopicID, int nSequenceNo);
    void OnPackageEnd(int nTopicID, int nSequenceNo);
    void OnRspError(CUstpFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogin(CUstpFtdcRspUserLoginField* pRspUserLogin, CUstpFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel); 
    void OnRtnTrade(CUstpFtdcTradeField *pTrade); 
    void OnRtnOrder(CUstpFtdcOrderField* pOrder); 
    void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo); 
    void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo); 
    void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus); 
    void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes); 
    void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote); 
    void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo); 
    void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo); 
    void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote);
    void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg); 
    void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction); 
    void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit); 
    void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
    void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRspQryInstrumentGroup(CUstpFtdcRspInstrumentGroupField *pRspInstrumentGroup, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRspQryClientMarginCombType(CUstpFtdcRspClientMarginCombTypeField *pRspClientMarginCombType, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRspExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRspExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
    void OnRtnExecOrder(CUstpFtdcExecOrderField *pExecOrder);  
    void OnErrRtnExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo);  
    void OnErrRtnExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo);  
    void OnRtnTransferMoney(CUstpFtdcSyncMoneyTransferField *pSyncMoneyTransfer);  
    void OnRspQrySystemTime(CUstpFtdcRspQrySystemTimeField *pRspQrySystemTime, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); 
private:
    CUstpFtdcTraderApi* t_pUserApi;
};

// M
class MspiSimpleHandler : public CUstpFtdcMduserSpi {
public:
    MspiSimpleHandler(CUstpFtdcMduserApi *pUserApi): m_pUserApi(pUserApi) {};
    ~MspiSimpleHandler() {};
	void OnFrontConnected();
	void OnFrontDisconnected(int nReason);
	void OnHeartBeatWarning(int nTimeLapse);
	void OnPackageStart(int nTopicID, int nSequenceNo);
	void OnPackageEnd(int nTopicID, int nSequenceNo);
	void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData);
	void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspGetMarketTopic(CUstpFtdcRspMarketTopicField *pRspMarketTopic, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspGetMarketData(CUstpFtdcRspDepthMarketDataField *pRspDepthMarketData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
private:
    CUstpFtdcMduserApi *m_pUserApi;
};








    
    