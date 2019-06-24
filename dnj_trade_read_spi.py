#!/usr/bin/python3
#coding=UTF-8


"""




"""

import sys
import time
import threading
import subprocess
import pprint

import dnj_trade_common
from dnj_trade_terminal import *

dnj = dnj_trade_common.dnj

# read_in_irr 子執行緒
def read_in_irr():
    dnj['tt']['pReadInIrr'] = dnj_trade_common.get_currentTT()
    # global dnj['cfg']
    read_in_irr_args = ["tail", "-f", "-n1", dnj['cfg']['filenamee_write_result']]
    sub_read_in_irr = subprocess.Popen(read_in_irr_args, bufsize=0, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
    while True:  
        buff1 = sub_read_in_irr.stdout.readline().decode("utf-8").strip()
        #buff1 = sub_read_in_irr.stdout.readline()
        tt_1 = dnj_trade_common.get_currentTT()
        time_1 = tt_1['timestamp_micro']
        if buff1 == '' and sub_read_in_irr.poll() != None:  
            continue
        else:
            parse_in_irr(buff1)

# read_rsp 子執行緒
def read_rsp():

    dnj['tt']['pReadRSP'] = dnj_trade_common.get_currentTT()
    # global dnj['cfg']
    read_rsp_args = ["tail", "-f", "-n1", dnj['cfg']['filenamee_read_rsp']]
    read_rsp = subprocess.Popen(read_rsp_args, bufsize=0, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
    while True:  
        buff1 = read_rsp.stdout.readline().decode("utf-8")
        #buff1 = read_rsp.stdout.readline()
        tt_1 = dnj_trade_common.get_currentTT()
        time_1 = tt_1['timestamp_micro']
        if buff1 == '' and read_rsp.poll() != None:  
            continue
        else:
            parse_rsp(buff1)

# read_rtn 子執行緒
def read_rtn():
    dnj['tt']['pReadRTN'] = dnj_trade_common.get_currentTT()
    # global dnj['cfg']
    read_rtn_args = ["tail", "-f", "-n1", dnj['cfg']['filenamee_read_rtn']]
    read_rtn = subprocess.Popen(read_rtn_args, bufsize=0, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
    while True:  
        #buff1 = read_rtn.stdout.readline().decode("utf-8")
        buff1 = read_rtn.stdout.readline()
        tt_1 = dnj_trade_common.get_currentTT()
        time_1 = tt_1['timestamp_micro']
        if buff1 == '' and read_rtn.poll() != None:  
            continue
        else:
            parse_rtn(buff1)

# read_rtn_trr22 子執行緒
def read_rtn_trr22():
    dnj['tt']['pReadRTNtrr22'] = dnj_trade_common.get_currentTT()
    #global dnj['cfg']
    read_rtn_trr22_args = ["tail", "-f", "-n1", dnj['cfg']['filenamee_read_rtn_trr22']]
    read_rtn_trr22 = subprocess.Popen(read_rtn_trr22_args, bufsize=0, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
    while True:  
        buff2 = read_rtn_trr22.stdout.readline().decode("utf-8")
        #buff2 = read_rtn_trr22.stdout.readline()
        if buff2 == '' and read_rtn_trr22.poll() != None:  
            continue
        else:
            xxx_one = parse_trr22(buff2)
            #if depdata_one:
            #    deptmarketdata[depdata_one['InstrumentID']] = depdata_one
            
# read_rtn_mrr11 子執行緒
def read_rtn_mrr11():
    dnj['tt']['pReadRTNmr11'] = dnj_trade_common.get_currentTT()
    #global dnj['cfg']
    read_rtn_mrr11_args = ["tail", "-f", "-n1", dnj['cfg']['filenamee_read_rtn_mrr11']]
    read_rtn_mrr11 = subprocess.Popen(read_rtn_mrr11_args, bufsize=0, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
    while True:  
        buff2 = read_rtn_mrr11.stdout.readline().decode("utf-8")
        #buff2 = read_rtn_mrr11.stdout.readline()
        if buff2 == '' and read_rtn_mrr11.poll() != None:  
            continue
        else:
            depdata_one = parse_mrr11(buff2)
            if depdata_one:
                dnj_trade_common.deptmarketdata[depdata_one['InstrumentID']] = depdata_one.copy()
                tt_last = dnj_trade_common.get_currentTT()
                dnj_trade_common.rr_lastupdate['mrr11'] = tt_last["timestamp_micro"]
                terminal_update_mrr11(depdata_one);

def parse_in_irr (line_this):
    #global tqq16_irr
    fields = str(line_this).split(",")
    #pprint.pprint(fields)
    if fields[2] == "tqq16":
        dnj_trade_common.tqq16_irr = int(fields[8])
        tt_last = dnj_trade_common.get_currentTT()
        dnj_trade_common.rr_lastupdate['in_irr'] = tt_last["timestamp_micro"]
        dnj['check']['isJConnected'] = True 
        dnj['check']['isFConnected'] = True
        if dnj_trade_common.tqq16_irr == -1:
            dnj['check']['isFConnected'] = False 


        
        
        #print(tqq16_irr)
    #if fields[2] == "tqq16" and fields[8] == "-2":
    #    print("too fast!")
    #if fields[2] == "tqq16" and fields[8] == "-1":
    #    print("disconnect!")
    #if fields[2] == "tqq16" and fields[8] == "0":
    #    print("connect!")



def parse_rsp(line_this):
    fields = str(line_this).split(",")
    if len(fields) < 20:
#        print("rsp <20")
        return False
    if int(fields[ 1]) < int(dnj['tt']['Dstart']["timestamp_micro"]):
#        print ("rsp < Dstart")
        return False
    # trr12 OnRspOrderInsert 报单录入应答
    if fields[2] == "trr12":
        InputOrder_one = parse_trr12(fields) 
        if InputOrder_one:
            dnj_trade_common.InputOrder[InputOrder_one['UserOrderLocalID']] = InputOrder_one.copy()
    # trr13 OnRspOrderAction() 报单操作应答
    if fields[2] == "trr13":
        OrderAction_one = parse_trr13(fields) 
        if OrderAction_one:
            dnj_trade_common.OrderAction[OrderAction_one['UserOrderActionLocalID']] = OrderAction_one.copy()
    # trr35 OnRspQryOrder() 报单查询应答
    if fields[2] == "trr35":
        QueryOrder_one = parse_trr35(fields)
        #pprint.pprint(fields)
        if QueryOrder_one:
            dnj_trade_common.QueryOrder[QueryOrder_one['UserOrderLocalID']] = QueryOrder_one.copy()
    # trr39 OnRspQryInvestorAccount() 投资者资金账户查询应答 // much info
    if fields[2] == "trr39":
        InvestorAccount_one = parse_trr39(fields)
        #pprint.pprint(fields)
        if InvestorAccount_one:
            for key, value in InvestorAccount_one.items():
                dnj_trade_common.InvestorAccount[key] = value
    # trr40 OnRspQryInstrument 合约查询应答
    if fields[2] == "trr40":
        Instrument_one = parse_trr40(fields)
        #pprint.pprint(fields)
        if Instrument_one:
            dnj_trade_common.Instrument[Instrument_one['InstrumentID']] = Instrument_one.copy()
            tt_last = dnj_trade_common.get_currentTT()
            dnj_trade_common.rr_lastupdate['trr40'] = tt_last["timestamp_micro"]
            
    # trr42 OnRspQryInvestorPosition 投资者持仓查询应答  
    if fields[2] == "trr42":
        InvestorPosition_one = parse_trr42(fields)
        #pprint.pprint(fields)
        if InvestorPosition_one:
            dnj_trade_common.InvestorPosition[InvestorPosition_one['InstrumentID']+","+InvestorPosition_one['Direction']] = InvestorPosition_one.copy()
            tt_last = dnj_trade_common.get_currentTT()
            dnj_trade_common.rr_lastupdate['trr42'] = tt_last["timestamp_micro"]
            
            
            
            
    #
    #notify_terminal_update(fields[2])
    #return fields[1]


def parse_trr12(fields): # trr12 OnRspOrderInsert 报单录入应答
    InputOrder_this = {}
    InputOrder_this['r_datentime_micro'     ] = fields[ 0]
    InputOrder_this['r_timestamp_micro'     ] = fields[ 1]
    InputOrder_this['r_code'                ] = fields[ 2]
    InputOrder_this['r_name'                ] = fields[ 3]
    InputOrder_this['r_cmd'                 ] = fields[ 4]
    InputOrder_this['rsp_ErrorID'           ] = fields[ 5]
    InputOrder_this['rsp_ErrorMsg'          ] = fields[ 6]
    InputOrder_this['rsp_nRequestID'        ] = fields[ 7]
    InputOrder_this['rsp_bIsLast'           ] = fields[ 8]
    InputOrder_this['r_reserved_9'          ] = fields[ 9] 
    InputOrder_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    InputOrder_this['r_11'                  ] = fields[11] # r_11
    InputOrder_this['r_12'                  ] = fields[12] # r_12    
    InputOrder_this['r_13'                  ] = fields[13] # r_13    
    InputOrder_this['r_14'                  ] = fields[14] # r_14
    
    InputOrder_this['BrokerID'         ] = fields[20] # 经纪公司编号             %s
    InputOrder_this['ExchangeID'       ] = fields[21] # 交易所代码               %s
    InputOrder_this['OrderSysID'       ] = fields[22] # 系统报单编号             %s
    InputOrder_this['InvestorID'       ] = fields[23] # 投资者编号               %s
    InputOrder_this['UserID'           ] = fields[24] # 用户代码                 %s
    InputOrder_this['SeatNo'           ] = fields[25] # 指定通过此席位序号下单   %d
    InputOrder_this['InstrumentID'     ] = fields[26] # 合约代码/套利组合合约号  %s
    InputOrder_this['UserOrderLocalID' ] = fields[27] # 用户本地报单号           %s
    InputOrder_this['OrderPriceType'   ] = fields[28] # 报单类型                 %c
    InputOrder_this['Direction'        ] = fields[29] # 买卖方向                 %c
    InputOrder_this['OffsetFlag'       ] = fields[30] # 开平标志                 %c
    InputOrder_this['HedgeFlag'        ] = fields[31] # 投机套保标志             %c
    InputOrder_this['LimitPrice'       ] = fields[32] # 价格                     %f
    InputOrder_this['Volume'           ] = fields[33] # 数量                     %d
    InputOrder_this['TimeCondition'    ] = fields[34] # 有效期类型               %c
    InputOrder_this['GTDDate'          ] = fields[35] # GTD日期                  %s
    InputOrder_this['VolumeCondition'  ] = fields[36] # 成交量类型               %c
    InputOrder_this['MinVolume'        ] = fields[37] # 最小成交量               %d
    InputOrder_this['StopPrice'        ] = fields[38] # 止损价止赢价             %f
    InputOrder_this['ForceCloseReason' ] = fields[39] # 强平原因                 %c
    InputOrder_this['IsAutoSuspend'    ] = fields[40] # 自动挂起标志             %d
    InputOrder_this['BusinessUnit'     ] = fields[41] # 业务单元                 %s
    InputOrder_this['UserCustom'       ] = fields[42] # 用户自定义域             %s
    InputOrder_this['BusinessLocalID'  ] = fields[43] # 本地业务标识             %d  
    InputOrder_this['ActionDay'        ] = fields[44] # 业务发生日期             %s  
    InputOrder_this['ArbiType'         ] = fields[45] # 策略类别                 %c
    InputOrder_this['ClientID'         ] = fields[46] # 客户编码                 %s
    return InputOrder_this
    
def parse_trr13(fields): # trr13 OnRspOrderAction() 报单操作应答
    OrderAction_this = {}
    OrderAction_this['r_datentime_micro'     ] = fields[ 0]
    OrderAction_this['r_timestamp_micro'     ] = fields[ 1]
    OrderAction_this['r_code'                ] = fields[ 2]
    OrderAction_this['r_name'                ] = fields[ 3]
    OrderAction_this['r_cmd'                 ] = fields[ 4]
    OrderAction_this['rsp_ErrorID'           ] = fields[ 5]
    OrderAction_this['rsp_ErrorMsg'          ] = fields[ 6]
    OrderAction_this['rsp_nRequestID'        ] = fields[ 7]
    OrderAction_this['rsp_bIsLast'           ] = fields[ 8]
    OrderAction_this['r_reserved_9'          ] = fields[ 9] 
    OrderAction_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    OrderAction_this['r_11'                  ] = fields[11] # r_11
    OrderAction_this['r_12'                  ] = fields[12] # r_12    
    OrderAction_this['r_13'                  ] = fields[13] # r_13    
    OrderAction_this['r_14'                  ] = fields[14] # r_14
    
    OrderAction_this['ExchangeID'            ] = fields[20] # 交易所代码                  %s           
    OrderAction_this['OrderSysID'            ] = fields[21] # 系统报单编号                %s                
    OrderAction_this['BrokerID'              ] = fields[22] # 经纪公司编号                %s                
    OrderAction_this['InvestorID'            ] = fields[23] # 投资者编号                  %s                    
    OrderAction_this['UserID'                ] = fields[24] # 用户代码                    %s                    
    OrderAction_this['UserOrderActionLocalID'] = fields[25] # 本次撤单操作的本地编号      %s   
    OrderAction_this['UserOrderLocalID'      ] = fields[26] # 被撤订单的本地报单编号      %s   
    OrderAction_this['ActionFlag'            ] = fields[27] # 报单操作标志（只支持删除）  %c 
    OrderAction_this['LimitPrice'            ] = fields[28] # 价格（暂不支持，保留域）    %f 
    OrderAction_this['VolumeChange'          ] = fields[29] # 数量变化（暂不支持，保留域）%d 
    OrderAction_this['BusinessLocalID'       ] = fields[30] # 本地业务标识                %d             
    OrderAction_this['ClientID'              ] = fields[31] # 客户编码                    %s   
    return OrderAction_this

def parse_trr35(fields): # trr35 OnRspQryOrder() 报单查询应答
    QueryOrder_this = {}
    QueryOrder_this['r_datentime_micro'     ] = fields[ 0]
    QueryOrder_this['r_timestamp_micro'     ] = fields[ 1]
    QueryOrder_this['r_code'                ] = fields[ 2]
    QueryOrder_this['r_name'                ] = fields[ 3]
    QueryOrder_this['r_cmd'                 ] = fields[ 4]
    QueryOrder_this['rsp_ErrorID'           ] = fields[ 5]
    QueryOrder_this['rsp_ErrorMsg'          ] = fields[ 6]
    QueryOrder_this['rsp_nRequestID'        ] = fields[ 7]
    QueryOrder_this['rsp_bIsLast'           ] = fields[ 8]
    QueryOrder_this['r_reserved_9'          ] = fields[ 9] 
    QueryOrder_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    QueryOrder_this['r_11'                  ] = fields[11] # r_11
    QueryOrder_this['r_12'                  ] = fields[12] # r_12    
    QueryOrder_this['r_13'                  ] = fields[13] # r_13    
    QueryOrder_this['r_14'                  ] = fields[14] # r_14
    if fields[10] == "EMPTY":
#        print("EMPTY")
        return False
    QueryOrder_this['BrokerID'        ] = fields[20] # 经纪公司编号             %s
    QueryOrder_this['ExchangeID'      ] = fields[21] # 交易所代码               %s
    QueryOrder_this['OrderSysID'      ] = fields[22] # 系统报单编号             %s
    QueryOrder_this['InvestorID'      ] = fields[23] # 投资者编号               %s
    QueryOrder_this['UserID'          ] = fields[24] # 用户代码                 %s
    QueryOrder_this['SeatNo'          ] = fields[25] # 指定通过此席位序号下单   %d
    QueryOrder_this['InstrumentID'    ] = fields[26] # 合约代码/套利组合合约号  %s
    QueryOrder_this['UserOrderLocalID'] = fields[27] # 用户本地报单号           %s
    QueryOrder_this['OrderPriceType'  ] = fields[28] # 报单类型                 %c
    QueryOrder_this['Direction'       ] = fields[29] # 买卖方向                 %c
    QueryOrder_this['OffsetFlag'      ] = fields[30] # 开平标志                 %c
    QueryOrder_this['HedgeFlag'       ] = fields[31] # 投机套保标志             %c
    QueryOrder_this['LimitPrice'      ] = fields[32] # 价格                     %f
    QueryOrder_this['Volume'          ] = fields[33] # 数量                     %d
    QueryOrder_this['TimeCondition'   ] = fields[34] # 有效期类型               %c
    QueryOrder_this['GTDDate'         ] = fields[35] # GTD日期                  %s
    QueryOrder_this['VolumeCondition' ] = fields[36] # 成交量类型               %c
    QueryOrder_this['MinVolume'       ] = fields[37] # 最小成交量               %d
    QueryOrder_this['StopPrice'       ] = fields[38] # 止损价止赢价             %f
    QueryOrder_this['ForceCloseReason'] = fields[39] # 强平原因                 %c
    QueryOrder_this['IsAutoSuspend'   ] = fields[40] # 自动挂起标志             %d
    QueryOrder_this['BusinessUnit'    ] = fields[41] # 业务单元                 %s
    QueryOrder_this['UserCustom'      ] = fields[42] # 用户自定义域             %s
    QueryOrder_this['BusinessLocalID' ] = fields[43] # 本地业务标识             %d  
    QueryOrder_this['ActionDay'       ] = fields[44] # 业务发生日期             %s  
    QueryOrder_this['ArbiType'        ] = fields[45] # 策略类别                 %c
    QueryOrder_this['TradingDay'      ] = fields[46] # 交易日                   %s
    QueryOrder_this['ParticipantID'   ] = fields[47] # 会员编号                 %s
    QueryOrder_this['OrderUserID'     ] = fields[48] # 最初下单用户编号         %s
    QueryOrder_this['ClientID'        ] = fields[49] # 客户编码                 %s
    QueryOrder_this['SeatID'          ] = fields[50] # 下单席位号               %s
    QueryOrder_this['InsertTime'      ] = fields[51] # 插入时间                 %s
    QueryOrder_this['OrderLocalID'    ] = fields[52] # 本地报单编号             %s
    QueryOrder_this['OrderSource'     ] = fields[53] # 报单来源                 %c
    QueryOrder_this['OrderStatus'     ] = fields[54] # 报单状态                 %c
    QueryOrder_this['CancelTime'      ] = fields[55] # 撤销时间                 %s
    QueryOrder_this['CancelUserID'    ] = fields[56] # 撤单用户编号             %s
    QueryOrder_this['VolumeTraded'    ] = fields[57] # 今成交数量               %d
    QueryOrder_this['VolumeRemain'    ] = fields[58] # 剩余数量                 %d
    QueryOrder_this['OrderType'       ] = fields[59] # 委托类型                 %c
    QueryOrder_this['DeliveryFlag'    ] = fields[60] # 期权对冲标识             %c
    return QueryOrder_this

def parse_trr39(fields): # trr39 OnRspQryInvestorAccount() 投资者资金账户查询应答 // much info
    InvestorAccount_this = {}
    InvestorAccount_this['r_datentime_micro'     ] = fields[ 0]
    InvestorAccount_this['r_timestamp_micro'     ] = fields[ 1]
    InvestorAccount_this['r_code'                ] = fields[ 2]
    InvestorAccount_this['r_name'                ] = fields[ 3]
    InvestorAccount_this['r_cmd'                 ] = fields[ 4]
    InvestorAccount_this['rsp_ErrorID'           ] = fields[ 5]
    InvestorAccount_this['rsp_ErrorMsg'          ] = fields[ 6]
    InvestorAccount_this['rsp_nRequestID'        ] = fields[ 7]
    InvestorAccount_this['rsp_bIsLast'           ] = fields[ 8]
    InvestorAccount_this['r_reserved_9'          ] = fields[ 9] 
    InvestorAccount_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    InvestorAccount_this['r_11'                  ] = fields[11] # r_11
    InvestorAccount_this['r_12'                  ] = fields[12] # r_12    
    InvestorAccount_this['r_13'                  ] = fields[13] # r_13    
    InvestorAccount_this['r_14'                  ] = fields[14] # r_14

    InvestorAccount_this['BrokerID'              ] = fields[20] # 经纪公司编号     0059            
    InvestorAccount_this['InvestorID'            ] = fields[21] # 投资者编号       990407          
    InvestorAccount_this['AccountID'             ] = fields[22] # 资金帐号         990407          
    InvestorAccount_this['PreBalance'            ] = fields[23] # 上次结算准备金   24998813.140000 
    InvestorAccount_this['Deposit'               ] = fields[24] # 入金金额         0.000000        
    InvestorAccount_this['Withdraw'              ] = fields[25] # 出金金额         0.000000        
    InvestorAccount_this['FrozenMargin'          ] = fields[26] # 冻结的保证金     0.000000        
    InvestorAccount_this['FrozenFee'             ] = fields[27] # 冻结手续费       0.000000        
    InvestorAccount_this['FrozenPremium'         ] = fields[28] # 冻结权利金       0.000000        
    InvestorAccount_this['Fee'                   ] = fields[29] # 手续费           0.000000        
    InvestorAccount_this['CloseProfit'           ] = fields[30] # 平仓盈亏         0.000000        
    InvestorAccount_this['PositionProfit'        ] = fields[31] # 持仓盈亏 (變)    -9840.000000    
    InvestorAccount_this['Available'             ] = fields[32] # 可用资金 (變)    24447378.340000 
    InvestorAccount_this['LongFrozenMargin'      ] = fields[33] # 多头冻结的保证金 0.000000        
    InvestorAccount_this['ShortFrozenMargin'     ] = fields[34] # 空头冻结的保证金 0.000000        
    InvestorAccount_this['LongMargin'            ] = fields[35] # 多头占用保证金   541594.800000   
    InvestorAccount_this['ShortMargin'           ] = fields[36] # 空头占用保证金   0.000000        
    InvestorAccount_this['ReleaseMargin'         ] = fields[37] # 当日释放保证金   0.000000        
    InvestorAccount_this['DynamicRights'         ] = fields[38] # 动态权益         0.000000        
    InvestorAccount_this['TodayInOut'            ] = fields[39] # 今日出入金       0.000000        
    InvestorAccount_this['Margin'                ] = fields[40] # 占用保证金       541594.800000   
    InvestorAccount_this['Premium'               ] = fields[41] # 期权权利金收支   0.000000        
    InvestorAccount_this['Risk'                  ] = fields[42] # 风险度           2.170000        
    return InvestorAccount_this                             
                                                                                                              
def parse_trr40(fields): # trr40 OnRspQryInstrument 合约查询应答
    Instrument_this = {}
    Instrument_this['r_datentime_micro'     ] = fields[ 0]
    Instrument_this['r_timestamp_micro'     ] = fields[ 1]
    Instrument_this['r_code'                ] = fields[ 2]
    Instrument_this['r_name'                ] = fields[ 3]
    Instrument_this['r_cmd'                 ] = fields[ 4]
    Instrument_this['rsp_ErrorID'           ] = fields[ 5]
    Instrument_this['rsp_ErrorMsg'          ] = fields[ 6]
    Instrument_this['rsp_nRequestID'        ] = fields[ 7]
    Instrument_this['rsp_bIsLast'           ] = fields[ 8]
    Instrument_this['r_reserved_9'          ] = fields[ 9] 
    Instrument_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    Instrument_this['r_11'                  ] = fields[11] # r_11
    Instrument_this['r_12'                  ] = fields[12] # r_12    
    Instrument_this['r_13'                  ] = fields[13] # r_13    
    Instrument_this['r_14'                  ] = fields[14] # r_14
    if fields[10] == "EMPTY":
#        print("EMPTY")
        return False
    Instrument_this['ExchangeID'           ] = fields[20] # 交易所代码        %s  CFFEX        CFFEX                   
    Instrument_this['ProductID'            ] = fields[21] # 品种代码          %s  IF           IO                       
    Instrument_this['ProductName'          ] = fields[22] # 品种名称          %s                                        
    Instrument_this['InstrumentID'         ] = fields[23] # 合约代码          %s  IF1905       IO1905-C-3900                
    Instrument_this['InstrumentName'       ] = fields[24] # 合约名称          %s  股指1905     股指期权1905-C-3900          
    Instrument_this['DeliveryYear'         ] = fields[25] # 交割年份          %d  2019         2019                  
    Instrument_this['DeliveryMonth'        ] = fields[26] # 交割月            %d  5            5                     
    Instrument_this['MaxLimitOrderVolume'  ] = fields[27] # 限价单最大下单量  %d  20           100                   
    Instrument_this['MinLimitOrderVolume'  ] = fields[28] # 限价单最小下单量  %d  1            1                     
    Instrument_this['MaxMarketOrderVolume' ] = fields[29] # 市价单最大下单量  %d  10           0                     
    Instrument_this['MinMarketOrderVolume' ] = fields[30] # 市价单最小下单量  %d  1            1                             
    Instrument_this['VolumeMultiple'       ] = fields[31] # 数量乘数          %d  300          100                   
    Instrument_this['PriceTick'            ] = fields[32] # 报价单位          %f  0.200000     0.200000              
    Instrument_this['Currency'             ] = fields[33] # 币种              %c  1            1                     
    Instrument_this['LongPosLimit'         ] = fields[34] # 多头限仓          %d  0            0                         
    Instrument_this['ShortPosLimit'        ] = fields[35] # 空头限仓          %d  0            0                             
    Instrument_this['LowerLimitPrice'      ] = fields[36] # 跌停板价          %f  3510.000000  0.200000                      
    Instrument_this['UpperLimitPrice'      ] = fields[37] # 涨停板价          %f  4289.600000  577.200000            
    Instrument_this['PreSettlementPrice'   ] = fields[38] # 昨结算            %f  3899.800000  188.400000            
    Instrument_this['InstrumentStatus'     ] = fields[39] # 合约交易状态      %c  2            1                     
    Instrument_this['CreateDate'           ] = fields[40] # 创建日            %s  20190315     20190222              
    Instrument_this['OpenDate'             ] = fields[41] # 上市日            %s  20190318     20190225              
    Instrument_this['ExpireDate'           ] = fields[42] # 到期日            %s  20190517     20190517              
    Instrument_this['StartDelivDate'       ] = fields[43] # 开始交割日        %s  20190517     20190517              
    Instrument_this['EndDelivDate'         ] = fields[44] # 最后交割日        %s  20190517     20190517               
    Instrument_this['BasisPrice'           ] = fields[45] # 挂牌基准价        %f  3745.600000  27.200000                   
    Instrument_this['IsTrading'            ] = fields[46] # 当前是否交易      %d  1            1                             
    Instrument_this['UnderlyingInstrID'    ] = fields[47] # 基础商品代码      %s  IF           IO1905                            
    Instrument_this['UnderlyingMultiple'   ] = fields[48] # 基础商品乘数      %d  1            1                                 
    Instrument_this['PositionType'         ] = fields[49] # 持仓类型          %c  2            2                     
    Instrument_this['StrikePrice'          ] = fields[50] # 执行价            %f  -            3900.000000           
    Instrument_this['OptionsType'          ] = fields[51] # 期权类型          %c  0            1                     
    Instrument_this['CurrencyID'           ] = fields[52] # 币种代码          %s                                     
    Instrument_this['ArbiType'             ] = fields[53] # 策略类别          %c  0            0                     
    Instrument_this['InstrumentID_1'       ] = fields[54] # 第一腿合约代码    %d  1422543471   1422543471                   
    Instrument_this['Direction_1'          ] = fields[55] # 第一腿买卖方向    %c  0            0                     
    Instrument_this['Ratio_1'              ] = fields[56] # 第一腿数量比例    %f  0.000000     0.000000                   
    Instrument_this['InstrumentID_2'       ] = fields[57] # 第二腿合约代码    %d  1422543512   1422543512                      
    Instrument_this['Direction_2'          ] = fields[58] # 第二腿买卖方向    %c  1            1                     
    Instrument_this['Ratio_2'              ] = fields[59] # 第二腿数量比例    %f  0.000000     0.000000                            
    return Instrument_this

def parse_trr42(fields): # OnRspQryInvestorPosition 投资者持仓查询应答  
    InvestorPosition_this = {}
    InvestorPosition_this['r_datentime_micro'     ] = fields[ 0]
    InvestorPosition_this['r_timestamp_micro'     ] = fields[ 1]
    InvestorPosition_this['r_code'                ] = fields[ 2]
    InvestorPosition_this['r_name'                ] = fields[ 3]
    InvestorPosition_this['r_cmd'                 ] = fields[ 4]
    InvestorPosition_this['rsp_ErrorID'           ] = fields[ 5]
    InvestorPosition_this['rsp_ErrorMsg'          ] = fields[ 6]
    InvestorPosition_this['rsp_nRequestID'        ] = fields[ 7]
    InvestorPosition_this['rsp_bIsLast'           ] = fields[ 8]
    InvestorPosition_this['r_reserved_9'          ] = fields[ 9] 
    InvestorPosition_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    InvestorPosition_this['r_11'                  ] = fields[11] # r_11
    InvestorPosition_this['r_12'                  ] = fields[12] # r_12    
    InvestorPosition_this['r_13'                  ] = fields[13] # r_13    
    InvestorPosition_this['r_14'                  ] = fields[14] # r_14
    if fields[10] == "EMPTY":
#        print("EMPTY")
        return False    
    InvestorPosition_this['InvestorID'         ] = fields[20] # /// 990407          投资者编号             TUstpFtdcInvestorIDType    
    InvestorPosition_this['BrokerID'           ] = fields[21] # /// 0059            经纪公司编号           TUstpFtdcBrokerIDType    
    InvestorPosition_this['ExchangeID'         ] = fields[22] # /// CFFEX           交易所代码             TUstpFtdcExchangeIDType    
    InvestorPosition_this['ClientID'           ] = fields[23] # /// 01568909        客户编码               TUstpFtdcClientIDType       
    InvestorPosition_this['InstrumentID'       ] = fields[24] # /// IO1905-C-3900   合约代码               TUstpFtdcInstrumentIDType   
    InvestorPosition_this['Direction'          ] = fields[25] # /// 0               买卖方向               TUstpFtdcDirectionType      
    InvestorPosition_this['HedgeFlag'          ] = fields[26] # /// 1               投机套保标志           TUstpFtdcHedgeFlagType    
    InvestorPosition_this['UsedMargin'         ] = fields[27] # /// 0.000000        优惠前占用保证金       TUstpFtdcMoneyType    
    InvestorPosition_this['Position'           ] = fields[28] # /// 2               今总持仓量             TUstpFtdcVolumeType        
    InvestorPosition_this['PositionCost'       ] = fields[29] # /// 41840.000000    今日持仓成本           TUstpFtdcPriceType        
    InvestorPosition_this['YdPosition'         ] = fields[30] # /// 2               昨持仓量               TUstpFtdcVolumeType         
    InvestorPosition_this['YdPositionCost'     ] = fields[31] # /// 41840.000000    昨日持仓成本           TUstpFtdcMoneyType        
    InvestorPosition_this['FrozenMargin'       ] = fields[32] # /// 0.000000        冻结的保证金           TUstpFtdcMoneyType        
    InvestorPosition_this['FrozenPosition'     ] = fields[33] # /// 0               开仓冻结持仓           TUstpFtdcVolumeType        
    InvestorPosition_this['FrozenClosing'      ] = fields[34] # /// 0               平仓冻结持仓           TUstpFtdcVolumeType        
    InvestorPosition_this['YdFrozenClosing'    ] = fields[35] # /// 0               平昨仓冻结持仓         TUstpFtdcVolumeType    
    InvestorPosition_this['FrozenPremium'      ] = fields[36] # /// 0.000000        冻结的权利金           TUstpFtdcMoneyType        
    InvestorPosition_this['LastTradeID'        ] = fields[37] # /// 0               最后一笔成交编号       TUstpFtdcTradeIDType
    InvestorPosition_this['LastOrderLocalID'   ] = fields[38] # /// 0               最后一笔本地报单编号   TUstpFtdcOrderLocalIDType   
    InvestorPosition_this['SpeculationPosition'] = fields[39] # /// 2               投机持仓量             TUstpFtdcVolumeType         
    InvestorPosition_this['ArbitragePosition'  ] = fields[40] # /// 0               套利持仓量             TUstpFtdcVolumeType         
    InvestorPosition_this['HedgePosition'      ] = fields[41] # /// 0               套保持仓量             TUstpFtdcVolumeType         
    InvestorPosition_this['SpecFrozenClosing'  ] = fields[42] # /// 0               投机平仓冻结量         TUstpFtdcVolumeType         
    InvestorPosition_this['HedgeFrozenClosing' ] = fields[43] # /// 0               套保平仓冻结量         TUstpFtdcVolumeType         
    InvestorPosition_this['Currency'           ] = fields[44] # /// CNY             币种                   TUstpFtdcCurrencyIDType                                      
    return InvestorPosition_this                          

def parse_rtn(line_this):
    return False
    
def parse_trr22(line_this):
    return False
    
# mrr11 OnRtnDepthMarketData 深度行情通知
def parse_mrr11(line_this): 
    fields = str(line_this).split(",")
	# fields[  ] LastPrice: 3709.800000
	# fields[  ] Volume:     141
	# fields[  ] BidPrice1: 3709.200000
	# fields[  ] BidVolume1: 1
	# fields[  ] AskPrice1: 3710.000000
	# fields[  ] AskVolume1: 5
	# fields[  ] InstrumentID: IF1903
	# fields[  ] UpdateTime: 09:30:30
	# fields[  ] UpdateMillisec: 800
    #print ("%d:%s" % (len(fields),  fields[42])) 
    if len(fields) < 50:
#        print("mrr11 <50")
        return False
    if int(fields[ 1]) < int(dnj['tt']['Dstart']["timestamp_micro"]):
#        print ("mrr11 < Dstart")
        return False
    depdata_this = {}
    depdata_this['r_datentime_micro'     ] = fields[ 0]
    depdata_this['r_timestamp_micro'     ] = fields[ 1]
    depdata_this['r_code'                ] = fields[ 2]
    depdata_this['r_name'                ] = fields[ 3]
    depdata_this['r_cmd'                 ] = fields[ 4]
    depdata_this['rsp_ErrorID'           ] = fields[ 5]
    depdata_this['rsp_ErrorMsg'          ] = fields[ 6]
    depdata_this['rsp_nRequestID'        ] = fields[ 7]
    depdata_this['rsp_bIsLast'           ] = fields[ 8]
    depdata_this['r_reserved_9'          ] = fields[ 9] 
    depdata_this['r_reserved_10'         ] = fields[10] # (could be 'EMPTY')
    depdata_this['r_11'                  ] = fields[11] # r_11
    depdata_this['r_12'                  ] = fields[12] # r_12    
    depdata_this['r_13'                  ] = fields[13] # r_13    
    depdata_this['r_14'                  ] = fields[14] # r_14
    
    depdata_this['TradingDay'        ] = fields[20] # 交易日          %s  20190301       
    depdata_this['SettlementGroupID' ] = fields[21] # 结算组代码      %s  SG01               
    depdata_this['SettlementID'      ] = fields[22] # 结算编号        %d  1                  
    depdata_this['PreSettlementPrice'] = fields[23] # 昨结算          %f  3677.000000        
    depdata_this['PreClosePrice'     ] = fields[24] # 昨收盘          %f  3678.000000        
    depdata_this['PreOpenInterest'   ] = fields[25] # 昨持仓量        %f  10694.000000       
    depdata_this['PreDelta'          ] = fields[26] # 昨虚实度        %f  -                  
    depdata_this['OpenPrice'         ] = fields[27] # 今开盘          %f  3707.200000        
    depdata_this['HighestPrice'      ] = fields[28] # 最高价          %f  3718.000000        
    depdata_this['LowestPrice'       ] = fields[29] # 最低价          %f  3707.200000        
    depdata_this['ClosePrice'        ] = fields[30] # 今收盘          %f  -                  
    depdata_this['UpperLimitPrice'   ] = fields[31] # 涨停板价        %f  4044.600000        
    depdata_this['LowerLimitPrice'   ] = fields[32] # 跌停板价        %f  3309.400000        
    depdata_this['SettlementPrice'   ] = fields[33] # 今结算          %f  -                  
    depdata_this['CurrDelta'         ] = fields[34] # 今虚实度        %f  -                  
    depdata_this['LastPrice'         ] = fields[35] # 最新价          %f  3709.800000        
    depdata_this['Volume'            ] = fields[36] # 数量            %d  141                
    depdata_this['Turnover'          ] = fields[37] # 成交金额        %f  156965820.000000   
    depdata_this['OpenInterest'      ] = fields[38] # 持仓量          %f  10695.000000       
    depdata_this['BidPrice1'         ] = fields[39] # 申买价一        %f  3709.200000        
    depdata_this['BidVolume1'        ] = fields[40] # 申买量一        %d  1                  
    depdata_this['AskPrice1'         ] = fields[41] # 申卖价一        %f  3710.000000        
    depdata_this['AskVolume1'        ] = fields[42] # 申卖量一        %d  5                  
    depdata_this['BidPrice2'         ] = fields[43] # 申买价二        %f  -                  
    depdata_this['BidVolume2'        ] = fields[44] # 申买量二        %d  0                  
    depdata_this['BidPrice3'         ] = fields[45] # 申买价三        %f  -                  
    depdata_this['BidVolume3'        ] = fields[46] # 申买量三        %d  0                  
    depdata_this['AskPrice2'         ] = fields[47] # 申卖价二        %f  -                  
    depdata_this['AskVolume2'        ] = fields[48] # 申卖量二        %d  0                  
    depdata_this['AskPrice3'         ] = fields[49] # 申卖价三        %f  -                  
    depdata_this['AskVolume3'        ] = fields[50] # 申卖量三        %d  0                  
    depdata_this['BidPrice4'         ] = fields[51] # 申买价四        %f  -                  
    depdata_this['BidVolume4'        ] = fields[52] # 申买量四        %d  0                  
    depdata_this['BidPrice5'         ] = fields[53] # 申买价五        %f  -                  
    depdata_this['BidVolume5'        ] = fields[54] # 申买量五        %d  0                  
    depdata_this['AskPrice4'         ] = fields[55] # 申卖价四        %f  -                  
    depdata_this['AskVolume4'        ] = fields[56] # 申卖量四        %d  0                  
    depdata_this['AskPrice5'         ] = fields[57] # 申卖价五        %f  -                  
    depdata_this['AskVolume5'        ] = fields[58] # 申卖量五        %d  0                  
    depdata_this['InstrumentID'      ] = fields[59] # 合约代码        %s  IF1903             
    depdata_this['UpdateTime'        ] = fields[60] # 最后修改时间    %s  09:30:30           
    depdata_this['UpdateMillisec'    ] = fields[61] # 最后修改毫秒    %d  800                
    depdata_this['ActionDay'         ] = fields[62] # 业务发生日期    %s                     
    depdata_this['HisHighestPrice'   ] = fields[63] # 历史最高价      %f  0.000000           
    depdata_this['HisLowestPrice'    ] = fields[64] # 历史最低价      %f  0.000000           
    depdata_this['LatestVolume'      ] = fields[65] # 最新成交量      %d  0                  
    depdata_this['InitVolume'        ] = fields[66] # 初始持仓量      %d  0                  
    depdata_this['ChangeVolume'      ] = fields[67] # 持仓量变化      %d  0                  
    depdata_this['BidImplyVolume'    ] = fields[68] # 申买推导量      %d  0                  
    depdata_this['AskImplyVolume'    ] = fields[69] # 申卖推导量      %d  0                  
    depdata_this['AvgPrice'          ] = fields[70] # 当日均价        %f  0.000000           
    depdata_this['ArbiType'          ] = fields[71] # 策略类别        %c                     
    depdata_this['InstrumentID_1'    ] = fields[72] # 第一腿合约代码  %s                     
    depdata_this['InstrumentID_2'    ] = fields[73] # 第二腿合约代码  %s                     
    depdata_this['InstrumentName'    ] = fields[74] # 合约名称        %s                     
    depdata_this['TotalBidVolume'    ] = fields[75] # 总买入量        %d  0              
    depdata_this['TotalAskVolume'    ] = fields[76] # 总卖出量        %d  0    
    return depdata_this
    
    
    
    
if __name__ == '__main__':
    print ("123")

