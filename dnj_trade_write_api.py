#!/usr/bin/python3
#coding=UTF-8


"""


def form_tqq17(ProductID, InstrumentID): # tqq17:ReqQryInstrument() 合约查询请求

"""

import dnj_trade_common

dnj = dnj_trade_common.dnj


# tqq04 ReqOrderInsert() 发出报单录入请求
# expect: trr12 OnRspOrderInsert 报单录入应答
# expect: trr22 OnRtnOrder() 报单回报 // much info  
# expect: trr21 OnRtnTrade() 成交回报
# expect: trr23 OnErrRtnOrderInsert() 报单录入错误回报
def form_tqq04(InstrumentID, OrderPriceType, Direction, OffsetFlag, LimitPrice, Volume, TimeCondition, VolumeCondition):
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq04"                        ) #2
    fields.append( "ReqOrderInsert()"             ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gBrokerID']      )  # "0059"     BrokerID         ///经纪公司编号,必填字段                                                     
    fields.append( dnj['cfg']['gExchangeID']    )  # "CFFEX"    ExchangeID       ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）                 
    fields.append( "~X~"                        )  # ""         OrderSysID       ///系统报单编号                                                              
    fields.append( dnj['cfg']['gInvestorID']    )  # "990407"   InvestorID       ///投资者编号,必填字段 （客户资金账户）                                      
    fields.append( dnj['cfg']['gUserID']        )  # "990407"   UserID           //用户代码,必填字段（飞马用户代码）                                          
    fields.append( "~X~"                        )  # ""         SeatNo           ///指定下单席位编号（取值范围[1-N]，N为可用席位数目，超出范围的随机分配席位）
    fields.append( InstrumentID                 )  # "IF1904"   InstrumentID     ///合约代码,必填字段                                                         
    fields.append( str(tt_now["timestamp_micro"])) # "00001"    UserOrderLocalID ///用户本地报单号,必填字段                                                   
    fields.append( OrderPriceType               )  # '3'        OrderPriceType   ///(option 只能 限价2, 不能 任意價格市价1) 报单价格条件,必填字段（支持市价1、限价2、五档价4 和最优价3）
    fields.append( Direction                    )  # '0'        Direction        ///(# 0 买, 1 卖, 9 All) 买卖方向,必填字段                                                          
    fields.append( OffsetFlag                   )  # '0'        OffsetFlag       ///开平标志,必填字段（中金所支持开仓0和平仓1，上期所支持开仓、平今和平昨）     
    fields.append( '1'                          )  # '1'        HedgeFlag        ///投机套保标志,必填字段（支持投机、套保、套利、做市商编码）                 
    fields.append( LimitPrice                   )  # "500"      LimitPrice       ///价格,必填字段                                                             
    fields.append( Volume                       )  # '1'        Volume           ///数量,必填字段                                                             
    fields.append( TimeCondition                )  # '3'        TimeCondition    ///有效期类型,必填字段（“立即完成，否则撤销”1 OR“当日有效”3）                
    fields.append( "~X~"                        )  # ""         GTDDate          ///GTD日期（暂不支持，保留域）                                               
    fields.append( VolumeCondition              )  # '1'        VolumeCondition  ///成交量类型,必填字段（支持“任意数量 1” 和 “全部数量 3”）                         
    fields.append( "~X~"                        )  # ""         MinVolume        ///最小成交量（暂不支持，保留域）                                            
    fields.append( "~X~"                        )  # ""         StopPrice        ///止损价（暂不支持，保留域）                                                
    fields.append( '0'                          )  # '0'        ForceCloseReason ///强平原因，只支持”非强平” ? （暂不支持，保留域）                           
    fields.append( "~X~"                        )  # ""         IsAutoSuspend    ///自动挂起标志（暂不支持，保留域）                                          
    fields.append( "~X~"                        )  # ""         BusinessUnit     ///业务单元（暂不支持，保留域）                                              
    fields.append( "self"                       )  # "self"     UserCustom       ///用户自定义域 64字节                                                       
    fields.append( "~X~"                        )  # ""         BusinessLocalID  ///本地业务标识（暂不支持，保留域）                                          
    fields.append( "~X~"                        )  # ""         ActionDay        ///业务发生日期                                                              
    fields.append( "~X~"                        )  # ""         ArbiType         ///策略类别（套利组合业务必填字段）                                          
    fields.append( dnj['cfg']['gClientID']      )  # "01568909" ClientID         ///客户编码                                                                  
    return fields


# tqq05 ReqOrderAction() 报单操作请求
# expect: trr13 OnRspOrderAction() 报单操作应答
def form_tqq05(OrderSysID, UserOrderLocalID):
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq05"                        ) #2
    fields.append( "ReqOrderAction()"             ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gExchangeID']      ) # "CFFEX"        ExchangeID              ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）                 
    fields.append( OrderSysID                     ) #                OrderSysID              ///交易所系统报单编号（不为空按该字段操作，为空按本地报单编号操作） TUstpFtdcOrderSysIDType 
    fields.append( dnj['cfg']['gBrokerID']        ) # "0059"         BrokerID                ///经纪公司编号,必填字段                                                     
    fields.append( dnj['cfg']['gInvestorID']      ) # "990407"       InvestorID              ///投资者编号,必填字段 （客户资金账户）                                      
    fields.append( dnj['cfg']['gUserID']          ) # "990407"       UserID                  //用户代码,必填字段（飞马用户代码）                                          
    fields.append( str(tt_now["timestamp_micro"]) ) #  UserOrderActionLocalID  ///本次撤单Req的本地编号，必填字段 （需要保证按字典序递增） TUstpFtdcUserOrderLocalIDType # (直接用 timestamp, 一定比原來的 UserOrderLocalID 大)
    fields.append( UserOrderLocalID               ) #                UserOrderLocalID        ///被撤订单的本地报单编号                                   TUstpFtdcUserOrderLocalIDType
    fields.append( '0'                            ) #  "0"           ActionFlag              ///报单操作标志，必填字段（只支持删除）                     TUstpFtdcActionFlagType 
    fields.append( "~X~"                          ) #                LimitPrice              ///价格 （暂不支持，保留域）                                TUstpFtdcPriceType
    fields.append( "~X~"                          ) #                VolumeChange            ///数量变化（暂不支持，保留域）                             TUstpFtdcVolumeType
    fields.append( "~X~"                          ) #                BusinessLocalID         ///本地业务标识                                             TUstpFtdcBusinessLocalIDType
    fields.append( dnj['cfg']['gClientID']        ) # "01568909"     ClientID                ///客户编码                                                                  
    return fields

# tqq12 ReqQryOrder() 报单查询请求
# expect: trr35 OnRspQryOrder() 报单查询应答
def form_tqq12(OrderSysID, InstrumentID, OrderStatus):
    
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq12"                        ) #2
    fields.append( "ReqQryOrder()"                ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gBrokerID']      )  # "0059"     BrokerID        ///经纪公司编号,必填字段                                                     
    fields.append( dnj['cfg']['gUserID']        )  # "990407"   UserID          //用户代码,必填字段（飞马用户代码）                                          
    fields.append( "~X~"                        )  # 	        ExchangeID      ///交易所代码 TUstpFtdcExchangeIDType
    fields.append( dnj['cfg']['gInvestorID']    )  # "990407"   InvestorID      ///投资者编号,必填字段 （客户资金账户）                                      
    fields.append( OrderSysID                   )  # "~X~"      OrderSysID	    ///系统报单编号 TUstpFtdcOrderSysIDType
    fields.append( InstrumentID                 )  # "~X~"      InstrumentID    ///合约代码 TUstpFtdcInstrumentIDType
    fields.append( OrderStatus                  )  # ""         OrderStatus     ///报单状态 TUstpFtdcOrderStatusType
    fields.append( "~X~"                        )  #            OrderType	    ///委托类型 TUstpFtdcOrderTypeType
    fields.append( "~X~"                        )  #            ClientID	    ///客户编码 TUstpFtdcClientIDType
    return fields


# tqq16 ReqQryInvestorAccount() 投资者资金账户查询请求
# expect: trr39:OnRspQryInvestorAccount 投资者资金账户查询应答
def form_tqq16():
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq16"                        ) #2
    fields.append( "ReqQryInvestorAccount()"      ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gBrokerID']      )  # "0059"     BrokerID         ///经纪公司编号,必填字段                                                     
    fields.append( dnj['cfg']['gUserID']        )  # "990407"   UserID           //用户代码,必填字段（飞马用户代码）                                          
    fields.append( dnj['cfg']['gInvestorID']    )  # "990407"   InvestorID       ///投资者编号,必填字段 （客户资金账户）                                      
    return fields

# tqq17 ReqQryInstrument() 合约查询请求
# expect: trr40 OnRspQryInstrument 合约查询应答
def form_tqq17(ProductID, InstrumentID):
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq17"                        ) #2
    fields.append( "ReqQryInstrument()"           ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gExchangeID']      )# 交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）
    fields.append( ProductID                      )# 产品代码
    fields.append( InstrumentID                   )# 合约代码
    return fields

# tqq19 ReqQryInvestorPosition() 投资者持仓查询请求
# expect: trr42 OnRspQryInvestorPosition 投资者持仓查询应答  
def form_tqq19(InstrumentID):
    # should clear InvestorPosition first
    if InstrumentID == "":
        dnj_trade_common.InvestorPosition.clear()
    else:
        key0 = InstrumentID + ",0"
        key1 = InstrumentID + ",1"
        if key0 in dnj_trade_common.InvestorPosition:
            del dnj_trade_common.InvestorPosition[key0]
        if key1 in dnj_trade_common.InvestorPosition:
            del dnj_trade_common.InvestorPosition[key1]
    tt_now = dnj_trade_common.get_currentTT()
    fields = []
    # first 10 reserved fields
    fields.append( tt_now["datentime_micro"]      ) #0
    fields.append( str(tt_now["timestamp_micro"]) ) #1, UserOrderLocalID (char [21]) (if needed)
    fields.append( "tqq19"                        ) #2
    fields.append( "ReqQryInvestorPosition()"     ) #3
    fields.append( "tcmd"                         ) #4
    fields.append( "in"                           ) #5, co(console manual)/au(auto)/in(in.txt)
    fields.append( "in001"                        ) #4
    fields.append( tt_now["nRequestID"]           ) #7 nRequestID (int), 1553[826774366]354
    for x in range(12): # 12times (#8 - #19)
        fields.append( ""                             )
    # data start from [#20], 21st fields
    fields.append( dnj['cfg']['gBrokerID']      )  # "0059"     BrokerID         ///经纪公司编号,必填字段                                                     
    fields.append( dnj['cfg']['gUserID']        )  # "990407"   UserID           //用户代码,必填字段（飞马用户代码）                                          
    fields.append( dnj['cfg']['gExchangeID']    )  # "CFFEX"    ExchangeID       ///交易所代码,必填字段（支持中金所、上期所、大商所、郑商所）                 
    fields.append( dnj['cfg']['gInvestorID']    )  # "990407"   InvestorID       ///投资者编号,必填字段 （客户资金账户）                                      
    fields.append( InstrumentID                 )  # ""         InstrumentID     ///合约代码, 可以填寫 空字串 (回傳全部)
    return fields


def write_in(fields):
    
    q_11 = dnj_trade_common.get_currentTT()
    fields[11] = str(q_11["timestamp_micro"]);
    str_app = ""
    for idx in range(0, len(fields)):
        #print (">%d:%s<" %(idx,fields[idx]))
        str_app += fields[idx] + "," #0
    # print(str_app)
    if dnj_trade_common.fh_filenamee_write.closed:
        dnj_trade_common.fh_filenamee_write = open(dnj['cfg']['filenamee_write'], "a")
#        print (">>>>>>>>>>>>>>>>do a open: %s" %(dnj['cfg']['filenamee_write']))
    dnj_trade_common.fh_filenamee_write.write(str_app + "\n")
    dnj_trade_common.fh_filenamee_write.flush()
    #dnj_trade_common.fh_filenamee_write.close()
    q_12 = dnj_trade_common.get_currentTT()
    #print ("%s (編號:%s) 全部p:%6d (mq前:%6d, mq t12-t11:%5d) (q12:%d [網路])"  %(fields[2], fields[1], q_12["timestamp_micro"]-int(fields[1]), q_11["timestamp_micro"] - int(fields[1]), q_12["timestamp_micro"] - q_11["timestamp_micro"], q_12["timestamp_micro"]))
    
    