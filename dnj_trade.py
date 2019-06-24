#!/usr/bin/python3
#coding=UTF-8


"""




"""

import time
import threading
import dnj_trade_common
from dnj_trade_read_spi import read_in_irr,read_rsp,read_rtn,read_rtn_trr22,read_rtn_mrr11
from dnj_trade_write_api import write_in,form_tqq12,form_tqq16,form_tqq19
from dnj_trade_terminal import terminal_main,terminal_update_msg,terminal_update_act_info,terminal_update_connectivity,print_time
from dnj_trade_strategy import strategy_main

dnj = dnj_trade_common.dnj

# program start
dnj['tt']['Dstart'] = dnj_trade_common.get_currentTT()

# should be load config 
isLoadCFG = dnj_trade_common.load_cfg_init()
if not isLoadCFG:
    print ("error reading cfg")
    sys.exit(0)

# print_time 子執行緒 (只是 print time, 和 keep connection)
t_print_time = threading.Thread(target = print_time)
t_print_time.start()

# read_in_irr 子執行緒 (這個可以當作 qq 的 log)
t_read_in_irr    = threading.Thread(target = read_in_irr)
t_read_in_irr.start()

# read_rsp 子執行緒
t_read_rsp       = threading.Thread(target = read_rsp)
t_read_rsp.start()

# read_rtn 子執行緒
t_read_rtn       = threading.Thread(target = read_rtn)
t_read_rtn.start()

# read_rtn_trr22 子執行緒
t_read_rtn_trr22 = threading.Thread(target = read_rtn_trr22)
t_read_rtn_trr22.start()

# read_rtn_mrr11 子執行緒
t_read_rtn_mrr11 = threading.Thread(target = read_rtn_mrr11)
t_read_rtn_mrr11.start()


# terminal_main 子執行緒
t_terminal_main = threading.Thread(target = terminal_main)
t_terminal_main.start()

# strategy_main 子執行緒
t_strategy_main = threading.Thread(target = strategy_main)
t_strategy_main.start()



          



def check_connectivity():
    write_in(form_tqq16()) # tqq16 ReqQryInvestorAccount()
    tt_last = dnj_trade_common.get_currentTT()
    if (tt_last["timestamp_micro"] - dnj_trade_common.rr_lastupdate['in_irr']) > 10*1000000:
        dnj['check']['isJConnected'] = False
    else:
        dnj['check']['isJConnected'] = True
    if (tt_last["timestamp_micro"] - dnj_trade_common.rr_lastupdate['mrr11']) > 5*1000000:
        dnj['check']['isMrr11Coming'] = False
    else:
        dnj['check']['isMrr11Coming'] = True
    
while True:
    dnj_trade_common.QueryOrder.clear()
    check_connectivity()
    #write_in(form_tqq19("")) #不行, 會 -2
    terminal_update_connectivity()
    terminal_update_act_info()
    dic_len = ("tqq16_irr:%d, Inst:%d, dept:%d, pos:%d, InputOrder:%d, QueryOrder:%d, ordact:%d, acct:%d" 
        %(dnj_trade_common.tqq16_irr, 
          len(dnj_trade_common.Instrument.keys()), 
          len(dnj_trade_common.deptmarketdata.keys()), 
          len(dnj_trade_common.InvestorPosition.keys()), 
          len(dnj_trade_common.InputOrder.keys()), 
          len(dnj_trade_common.QueryOrder.keys()), 
          len(dnj_trade_common.OrderAction.keys()), 
          len(dnj_trade_common.InvestorAccount.keys()) 
          )
        )
    terminal_update_msg(31, 1, dic_len)
    time.sleep(7) 




    
    
    