#!/usr/bin/python3
#coding=UTF-8


"""




"""
import os 
import sys
import time

import subprocess
import pprint
from datetime import datetime


# those global 
tqq16_irr = -4
jstatus = {}
jstatus['J_connected'] = False
jstatus['FT_connected'] = False
jstatus['FM_connected'] = False
jstatus['FM_login'] = False
jstatus['FT_login'] = False

dnj = {}
# status
dnj['check'] = {} 
dnj['check']['isJConnected'] = False        # is dnj_trade.py connected to dnj_femas.out
dnj['check']['isFConnected'] = False        # is dnj_femas.out connected to femas AF api
dnj['check']['isMrr11Coming'] = False       # 
#
rr_lastupdate = {}
rr_lastupdate['trr40'] = 0
rr_lastupdate['trr42'] = 0
rr_lastupdate['mrr11'] = 0
rr_lastupdate['in_irr'] = 0

# time
dnj['tt'] = {} 
"""
dnj['tt']['Dstart']         # this dnj_trade.py program start
dnj['tt']['loadCFG']        # loadCFG
dnj['tt']['pReadInIrr']     # read_in_irr 子執行緒
dnj['tt']['pReadRSP']       # read_rsp 子執行緒
dnj['tt']['pReadRTN']       # read_rtn 子執行緒
dnj['tt']['pReadRTNtrr22']  # read_rtn_trr22 子執行緒 
dnj['tt']['pReadRTNmr11']   # read_rtn_mrr11 子執行緒
dnj['tt']['pTerminalMain']  # terminal_main 子執行緒
dnj['tt']['pStrategylMain'] # strategy_main 子執行緒
"""
# cfg, filenames, configure
dnj['cfg'] = {}
"""
dnj['cfg']['pathnamee'                ] = "/home/jie/c_data/20190415"
dnj['cfg']['filenamee_console'        ] = "/home/jie/c_data/20190415/co.txt"
dnj['cfg']['filenamee_console_result' ] = "/home/jie/c_data/20190415/co_irr.txt"
dnj['cfg']['filenamee_write'          ] = "/home/jie/c_data/20190415/in.txt"
dnj['cfg']['filenamee_write_result'   ] = "/home/jie/c_data/20190415/in_irr.txt"
dnj['cfg']['filenamee_read_rsp'       ] = "/home/jie/c_data/20190415/rr_rsp.txt"
dnj['cfg']['filenamee_read_rtn'       ] = "/home/jie/c_data/20190415/rr_rtn.txt"
dnj['cfg']['filenamee_read_rtn_trr22' ] = "/home/jie/c_data/20190415/rr_rtn_trr22.txt"
dnj['cfg']['filenamee_read_rtn_mrr11' ] = "/home/jie/c_data/20190415/rr_rtn_mrr11.txt"
"""
# those buck data
InputOrder = {}         # trr12 OnRspOrderInsert 报单录入应答
OrderAction = {}        # trr13 OnRspOrderAction() 报单操作应答
QueryOrder = {}         # trr35 OnRspQryOrder() 报单查询应答
InvestorAccount = {}    # trr39 OnRspQryInvestorAccount() 投资者资金账户查询应答 // much info
Instrument = {}         # trr40 OnRspQryInstrument 合约查询应答 (tqq17:ReqQryInstrument() 合约查询请求)
InvestorPosition = {}   # trr42 OnRspQryInvestorPosition 投资者持仓查询应答
deptmarketdata = {}     # mrr11 OnRtnDepthMarketData 深度行情通知

fh_filenamee_write = open("./__pydummy__", "a")


def load_cfg_init():

    # test
    tqq16_irr = -3

    fname_cfg = "./dnj_femas.cfg"
    if not os.path.exists(fname_cfg):
        return False
    with open(fname_cfg) as f:
        content = f.readlines()
    for line_this in content:
        fields = line_this.split("=")
        if len(fields) == 2 and fields[0].strip() != "" and fields[1].strip() != "":
            dnj['cfg'][fields[0].strip()] = fields[1].strip()
    dnj['cfg']['datenamee'                ] = get_currentTT()["date8"]
    dnj['cfg']['pathnamee'                ] = ("%s/%s" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_console'        ] = ("%s/%s/co.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_console_result' ] = ("%s/%s/co_irr.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_write'          ] = ("%s/%s/in.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_write_result'   ] = ("%s/%s/in_irr.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_read_rsp'       ] = ("%s/%s/rr_rsp.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_read_rtn'       ] = ("%s/%s/rr_rtn.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_read_rtn_trr22' ] = ("%s/%s/rr_rtn_trr22.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    dnj['cfg']['filenamee_read_rtn_mrr11' ] = ("%s/%s/rr_rtn_mrr11.txt" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
    if not os.path.exists(dnj['cfg']['pathnamee']):
        os.makedirs(dnj['cfg']['pathnamee'])
    if not os.path.exists(dnj['cfg']['filenamee_write']):
        open(dnj['cfg']['filenamee_write'], 'a').close()
    if not os.path.exists(dnj['cfg']['filenamee_read_rsp']):
        open(dnj['cfg']['filenamee_read_rsp'], 'a').close()
    if not os.path.exists(dnj['cfg']['filenamee_read_rtn']):
        open(dnj['cfg']['filenamee_read_rtn'], 'a').close()
    if not os.path.exists(dnj['cfg']['filenamee_read_rtn_trr22']):
        open(dnj['cfg']['filenamee_read_rtn_trr22'], 'a').close()
    if not os.path.exists(dnj['cfg']['filenamee_read_rtn_mrr11']):
        open(dnj['cfg']['filenamee_read_rtn_mrr11'], 'a').close()

    #global fh_filenamee_write
    fh_filenamee_write.close()

    # init
    jstatus['J_connected'] = False
    jstatus['FT_connected'] = False
    jstatus['FM_connected'] = False
    jstatus['FM_login'] = False
    jstatus['FT_login'] = False
    # dnj['check']['isJConnected'] = False        # is dnj_trade.py connected to dnj_femas.out
    # dnj['check']['isFConnected'] = False        # is dnj_femas.out connected to femas AF api
    # dnj['check']['isMrr11Coming'] = False       # 

    dnj['tt']['loadCFG'] = get_currentTT()
    return True

def get_currentTT():
    #time_now = ("%.6f" % round(time.time(), 6)) # 1553813319.808830 string
    timestamp_f = round(time.time(), 6)          # 1553813319.80883  float
    tv_this = {}
    tv_this["time_t"     ] = int(timestamp_f)
    tv_this["suseconds_t"] = int(("%.6f" % timestamp_f)[11:])
    tt_this = {}
    tt_this["tv"             ] = tv_this
    tt_this["timestamp"      ] = int(timestamp_f)
    tt_this["timestamp_micro"] = round(timestamp_f * 1000000)
    tt_this["nRequestID"     ] = str(int(timestamp_f * 1000))[4:]
    tt_this["date"           ] = datetime.fromtimestamp(timestamp_f).strftime("%Y/%m/%d")
    tt_this["date8"          ] = datetime.fromtimestamp(timestamp_f).strftime("%Y%m%d")
    tt_this["time"           ] = datetime.fromtimestamp(timestamp_f).strftime("%H:%M:%S")
    tt_this["datentime"      ] = datetime.fromtimestamp(timestamp_f).strftime("%Y/%m/%d %H:%M:%S")
    tt_this["datentime_micro"] = datetime.fromtimestamp(timestamp_f).strftime("%Y/%m/%d %H:%M:%S %f")
    return tt_this


    

    
if __name__ == '__main__':
    print ("abc")
    tt_test = get_currentTT()
    print (tt_test)
