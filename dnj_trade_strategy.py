#!/usr/bin/python3
#coding=UTF-8


"""




"""
import os
import sys
import threading
import time
import pprint

import dnj_trade_common
from dnj_trade_write_api import write_in,form_tqq04,form_tqq05,form_tqq12,form_tqq17,form_tqq19
from dnj_trade_terminal import terminal_update_msg,update_IF_print_key,update_IO_C_print_key,update_IO_P_print_key

dnj = dnj_trade_common.dnj

ta_IF = ""
ta_IO_C = ""
ta_IO_P = ""
list_IO_aaa_C = []
list_IO_aaa_P = []

def strategy_main():
    dnj['tt']['pStrategylMain'] = dnj_trade_common.get_currentTT()
    
    global ta_IF, ta_IO_C, ta_IO_P, list_IO_aaa_C, list_IO_aaa_P
    
    # make sure connection
    
    # get IF* contracts, find the most recently (IF1095) (1095)
    # from IF1095, today's quote (mrr11), find the "pivot", e.g. 4098
    # get IO1095-*, find up/down 5 contracts near 4098 (for print)
    t_update_ta_IF = 0
    t_update_position = 0
    while True:
        tt_now = dnj_trade_common.get_currentTT()
        
        # update_position_list()
        # tqq19 ReqQryInvestorPosition() 投资者持仓查询请求
        # expect: trr42 OnRspQryInvestorPosition 投资者持仓查询应答
        if (tt_now["timestamp_micro"] - t_update_position) > 9*1000000:
            YY_now = 18
            for key, value in dnj_trade_common.InvestorPosition.items():
                if not value["Position"] == "0":
                    msg = value["r_datentime_micro"] + " " + key + " Position:" + value["Position"]
                    terminal_update_msg(YY_now, 1, msg)
                    YY_now = YY_now + 1
            write_in(form_tqq19(""))
            t_update_position = tt_now["timestamp_micro"]


#        time.sleep(1)
        # update_IF_IO_list()
        # update if/io target
        update_IF_IO_list()
        msg = ("%s IF:%s, IOC:%s, IOP:%s, listIOC:%d, listIOP:%d" %(tt_now["datentime_micro"], ta_IF, ta_IO_C, ta_IO_P, len(list_IO_aaa_C), len(list_IO_aaa_P) ))
        terminal_update_msg(16, 1, msg)

        
        if (tt_now["timestamp_micro"] - t_update_ta_IF) > 10*59*1000000:
            if dnj_trade_common.dnj['check']['isMrr11Coming']:

                # tqq12 ReqQryOrder() 报单查询请求
                # expect: trr35 OnRspQryOrder() 报单查询应答
                write_in(form_tqq12("~X~", "~X~", "1"))
                time.sleep(1)
                write_in(form_tqq12("~X~", "~X~", "3"))
                time.sleep(5)
    
                # cancel all pending order
#                YY_now = 10
                for key, value in dnj_trade_common.QueryOrder.items():
                    # #define USTP_FTDC_OS_AllTraded '0' ///全部成交
                    # #define USTP_FTDC_OS_PartTradedQueueing '1' ///部分成交还在队列中
                    # #define USTP_FTDC_OS_PartTradedNotQueueing '2' ///部分成交不在队列中
                    # #define USTP_FTDC_OS_NoTradeQueueing '3' ///未成交还在队列中
                    # #define USTP_FTDC_OS_NoTradeNotQueueing '4' ///未成交不在队列中 (錯單)
                    # #define USTP_FTDC_OS_Canceled '5' ///撤单
                    # #define USTP_FTDC_OS_AcceptedNoReply '6' ///订单已报入交易所未应答
#                    msg = key + " OrderStatus:" + value['OrderStatus']
#                    terminal_update_msg(YY_now, 1, msg)
#                    YY_now = YY_now + 1
                    if value['OrderStatus'] == "1" or value['OrderStatus'] == "3":
                        write_in(form_tqq05("~X~", key))
    
                ## sell all
                for key, value in dnj_trade_common.InvestorPosition.items():
                    # form_tqq04(InstrumentID, OrderPriceType, Direction, OffsetFlag, LimitPrice, Volume, TimeCondition, VolumeCondition):
                    # InstrumentID     ///合约代码,必填字段
                    # OrderPriceType   ///(option 只能 限价2, 不能 任意價格市价1) 报单价格条件,必填字段（支持市价1、限价2、五档价4 和最优价3）
                    # Direction        ///(# 0 买, 1 卖, 9 All) 买卖方向,必填字段                                
                    # OffsetFlag       ///开平标志,必填字段（中金所支持开仓0和平仓1，上期所支持开仓、平今和平昨）
                    # LimitPrice       ///价格,必填字段                                              
                    # Volume           ///数量,必填字段                                              
                    # TimeCondition    ///有效期类型,必填字段（“立即完成，否则撤销”1 OR“当日有效”3） 
                    # VolumeCondition  ///成交量类型,必填字段（支持“任意数量 1” 和 “全部数量 3”）  
                    
                    if not value["Position"] == "0":
                    
                        InstrumentID_pos = value['InstrumentID']
                        Direction_pos = value['Direction']
                        Position_pos = value['Position']
                        FrozenClosing_pos = value['FrozenClosing']
                        FrozenPosition_pos = value['FrozenPosition']
                    
                        InstrumentID_order = InstrumentID_pos
                        OrderPriceType_order = "2"
                        Direction_order = "0" if Direction_pos == "1" else "1"
                        OffsetFlag_order = "1"
                        # LimitPrice_order
                        if InstrumentID_order.startswith("IO"): # option
                            if Direction_order == "1":  # 1 卖 (這裡故意賣 低價)
                                LimitPrice_order = dnj_trade_common.Instrument[InstrumentID_order]['LowerLimitPrice']
                            else:                       # 0 买 (這裡故意買 高價)
                                LimitPrice_order = dnj_trade_common.Instrument[InstrumentID_order]['UpperLimitPrice']
                        else: # future, 市價  +/- 30.0 保護帶 (use 29.0)
                            if Direction_order == "1":  # 1 卖 (這裡故意賣 低價)
                                LimitPrice_order = str(
                                    float(dnj_trade_common.deptmarketdata[InstrumentID_order]['LastPrice'])
                                    - 29
                            )
                            else:                       # 0 买 (這裡故意買 高價)
                                LimitPrice_order = str(
                                    float(dnj_trade_common.deptmarketdata[InstrumentID_order]['LastPrice'])
                                    + 29
                            ) 
                        Volume_order = str(int(Position_pos) - int(FrozenClosing_pos) - int(FrozenPosition_pos))
                        TimeCondition_order = "3"
                        VolumeCondition_order = "1"
                        write_in(form_tqq04(InstrumentID_order, OrderPriceType_order, Direction_order, OffsetFlag_order, LimitPrice_order, Volume_order, TimeCondition_order, VolumeCondition_order))
    
    
                # 上面的 sell 和 下面的 buy 會造成 
                # trr12,OnRspOrderInsert(),trsp,138,交易所返回:[订单触发自成交]
                time.sleep(8*60)
    
                # update if/io target
                update_IF_IO_list()
                msg = ("%s IF:%s, IOC:%s, IOP:%s, listIOC:%d, listIOP:%d" %(tt_now["datentime_micro"], ta_IF, ta_IO_C, ta_IO_P, len(list_IO_aaa_C), len(list_IO_aaa_P) ))
                terminal_update_msg(16, 1, msg)
    
                # buy 2 lots, for: ta_IF, ta_IO_C, ta_IO_P
                #for key in [ta_IF, ta_IO_C, ta_IO_P]:
                # future 損失比較多, 先不要
                for key in [ta_IO_C, ta_IO_P]:
                    # form_tqq04(InstrumentID, OrderPriceType, Direction, OffsetFlag, LimitPrice, Volume, TimeCondition, VolumeCondition):
                    # InstrumentID     ///合约代码,必填字段
                    # OrderPriceType   ///(option 只能 限价2, 不能 任意價格市价1) 报单价格条件,必填字段（支持市价1、限价2、五档价4 和最优价3）
                    # Direction        ///(# 0 买, 1 卖, 9 All) 买卖方向,必填字段                                
                    # OffsetFlag       ///开平标志,必填字段（中金所支持开仓0和平仓1，上期所支持开仓、平今和平昨）
                    # LimitPrice       ///价格,必填字段                                              
                    # Volume           ///数量,必填字段                                              
                    # TimeCondition    ///有效期类型,必填字段（“立即完成，否则撤销”1 OR“当日有效”3） 
                    # VolumeCondition  ///成交量类型,必填字段（支持“任意数量 1” 和 “全部数量 3”）  
                    InstrumentID_order = key
                    OrderPriceType_order = "2"
                    Direction_order = "0" 
                    OffsetFlag_order = "0"
                    # LimitPrice_order
                    if InstrumentID_order.startswith("IO"): # option
                        if Direction_order == "1":  # 1 卖 (這裡故意賣 低價)
                            LimitPrice_order = dnj_trade_common.Instrument[InstrumentID_order]['LowerLimitPrice']
                        else:                       # 0 买 (這裡故意買 高價)
                            LimitPrice_order = dnj_trade_common.Instrument[InstrumentID_order]['UpperLimitPrice']
                    else: # future, 市價  +/- 30.0 保護帶 (use 29.0)
                        if Direction_order == "1":  # 1 卖 (這裡故意賣 低價)
                            LimitPrice_order = str(
                                float(dnj_trade_common.deptmarketdata[InstrumentID_order]['LastPrice'])
                                - 29
                        )
                        else:                       # 0 买 (這裡故意買 高價)
                            LimitPrice_order = str(
                                float(dnj_trade_common.deptmarketdata[InstrumentID_order]['LastPrice'])
                                + 29
                        ) 
    
                    Volume_order = "2"
                    TimeCondition_order = "3"
                    VolumeCondition_order = "1"
                    write_in(form_tqq04(InstrumentID_order, OrderPriceType_order, Direction_order, OffsetFlag_order, LimitPrice_order, Volume_order, TimeCondition_order, VolumeCondition_order))
            
            # time
            t_update_ta_IF = tt_now["timestamp_micro"]
            

def update_IF_IO_list():
    global ta_IF, ta_IO_C, ta_IO_P, list_IO_aaa_C, list_IO_aaa_P
    # wait for connection and quote
    # IF
    write_in(form_tqq17("","IF"))
    list_IF = [key  for key in dnj_trade_common.Instrument.keys() if key.startswith("IF")];
    while len(list_IF) < 4:
        list_IF = [key  for key in dnj_trade_common.Instrument.keys() if key.startswith("IF")];
        list_IF.sort()
        time.sleep(3) 
        write_in(form_tqq17("","IF"))
    list_IF.sort()
    ta_IF = list_IF[0]
    ta_yr = ta_IF[2:]
    #print (ta_yr) #1905
    update_IF_print_key(list_IF)
    # IO
    ta_IO_prefix = "IO" + ta_yr
    write_in(form_tqq17("", ta_IO_prefix))
    time.sleep(2) 
    list_IO = [key  for key in dnj_trade_common.Instrument.keys() if key.startswith(ta_IO_prefix)];
    while len(list_IO) < 50:
        list_IO = [key  for key in dnj_trade_common.Instrument.keys() if key.startswith(ta_IO_prefix)];
        time.sleep(7) 
        write_in(form_tqq17("", ta_IO_prefix))
        
    while (ta_IF not in dnj_trade_common.deptmarketdata):
        time.sleep(1) 
    if dnj_trade_common.deptmarketdata[ta_IF]:
        ta_IF_quote = dnj_trade_common.deptmarketdata[ta_IF]['LastPrice']
    list_IO_aaa = [k  for k in list_IO if abs(int(float(dnj_trade_common.Instrument[k]['StrikePrice'])) - int(float(ta_IF_quote))) < 255]
    #print(len(list_IO_aaa))
    #pprint.pprint(list_IO_aaa)
    list_IO_aaa_C = [key  for key in list_IO_aaa if key.startswith(ta_IO_prefix + "-C-")];
    list_IO_aaa_C.sort()
    ta_IO_C = list_IO_aaa_C[4]
    update_IO_C_print_key(list_IO_aaa_C)
    list_IO_aaa_P = [key  for key in list_IO_aaa if key.startswith(ta_IO_prefix + "-P-")];
    list_IO_aaa_P.sort()
    ta_IO_P = list_IO_aaa_P[5]
    update_IO_P_print_key(list_IO_aaa_P)


if __name__ == '__main__':
    a = 5

