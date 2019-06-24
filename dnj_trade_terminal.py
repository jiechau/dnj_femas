#!/usr/bin/python3
#coding=UTF-8


"""

just for update print


"""
import os
import sys
import threading
import time
import pprint

import dnj_trade_common
from dnj_trade_write_api import *
from dnj_trade_read_spi import *

dnj = dnj_trade_common.dnj

BaseXX = 1
BaseYY = 1
# ta_IF
#list_IF = ()
#list_IO = ()
InstrumentYYff = {}
InstrumentYYioC = {}
InstrumentYYioP = {}

def terminal_main():

    dnj['tt']['pTerminalMain'] = dnj_trade_common.get_currentTT()
    print(chr(27) + "[2J") # clear the screen

    # update option

    # update trading
    
    # updat balance
    
    #print ("%d %d %d %d" %(len(Instrument.keys()), len(deptmarketdata.keys()), len(OrderAction.keys()), len(InputOrder.keys())))
    #time.sleep(1)

    # the inpu
    while True:
        line = get_input_str(28, 1)
        sys.stdout.write("\x1b7\x1b[%d;%df%-30s\x1b8" %(27, 1, "pre: " + line))
        sys.stdout.write("\x1b7\x1b[%d;%df%-30s\x1b8" %(28, 1, "cmd: "))
        sys.stdout.flush()

        line = line.strip()
        if line == "i":
            msg = "b:bool s:dict c:path 1:IF1905 2:IH 3:IO 4:tqq19"
            terminal_update_msg(29, 1, msg)
        if line == "b":
            msg = ("Femas:%s J:%s mrr11:%s                                     " %(str(dnj['check']['isFConnected']), str(dnj['check']['isJConnected']), str(dnj['check']['isMrr11Coming'])))
            terminal_update_msg(29, 1, msg)
        if line == "s":
            msg = ("%d %d %d %d %d" %(len(dnj_trade_common.Instrument.keys()), len(dnj_trade_common.deptmarketdata.keys()), len(dnj_trade_common.OrderAction.keys()), len(dnj_trade_common.InputOrder.keys()), len(dnj_trade_common.InvestorAccount.keys())))
            terminal_update_msg(29, 1, msg)
        if line == "1":
            write_in(form_tqq17("","IF1905"))
        if line == "2":
            write_in(form_tqq17("","IH"))
        if line == "3":
            write_in(form_tqq17("","IO"))
        if line == "4":
            write_in(form_tqq19(""))
        if line == "c":
            msg = ("%s/%s" % (dnj['cfg']['basenamee'], dnj['cfg']['datenamee']))
            terminal_update_msg(29, 1, msg)
        # pprint.pprint(Instrument)
        # pprint.pprint(deptmarketdata)
        # pprint.pprint(OrderAction)
        # pprint.pprint(InputOrder)
    
def notify_terminal_update(f_code, f_dict):
    print("notify_terminal_update -- %s" %(f_code))

def update_IF_print_key(list_IF):
    #global InstrumentYYff
    #InstrumentYYff = {list_IF[0]: BaseYY  , 
    #                  list_IF[1]: BaseYY+1, 
    #                  list_IF[2]: BaseYY+2, 
    #                  list_IF[3]: BaseYY+3}
    InstrumentYYff[list_IF[0]] = BaseYY
    InstrumentYYff[list_IF[1]] = BaseYY+1
    InstrumentYYff[list_IF[2]] = BaseYY+2
    InstrumentYYff[list_IF[3]] = BaseYY+3
                      
def update_IO_C_print_key(list_IO_aaa_C):
    InstrumentYYioC[list_IO_aaa_C[0]] = BaseYY
    InstrumentYYioC[list_IO_aaa_C[1]] = BaseYY+1
    InstrumentYYioC[list_IO_aaa_C[2]] = BaseYY+2
    InstrumentYYioC[list_IO_aaa_C[3]] = BaseYY+3
    InstrumentYYioC[list_IO_aaa_C[4]] = BaseYY+4
    InstrumentYYioC[list_IO_aaa_C[5]] = BaseYY+5
    InstrumentYYioC[list_IO_aaa_C[6]] = BaseYY+6
    InstrumentYYioC[list_IO_aaa_C[7]] = BaseYY+7
    InstrumentYYioC[list_IO_aaa_C[8]] = BaseYY+8
    InstrumentYYioC[list_IO_aaa_C[9]] = BaseYY+9
def update_IO_P_print_key(list_IO_aaa_P):
    InstrumentYYioP[list_IO_aaa_P[0]] = BaseYY
    InstrumentYYioP[list_IO_aaa_P[1]] = BaseYY+1 
    InstrumentYYioP[list_IO_aaa_P[2]] = BaseYY+2 
    InstrumentYYioP[list_IO_aaa_P[3]] = BaseYY+3
    InstrumentYYioP[list_IO_aaa_P[4]] = BaseYY+4 
    InstrumentYYioP[list_IO_aaa_P[5]] = BaseYY+5
    InstrumentYYioP[list_IO_aaa_P[6]] = BaseYY+6 
    InstrumentYYioP[list_IO_aaa_P[7]] = BaseYY+7
    InstrumentYYioP[list_IO_aaa_P[8]] = BaseYY+8 
    InstrumentYYioP[list_IO_aaa_P[9]] = BaseYY+9

def terminal_update_mrr11(depdata_one):  
    LastPrice      = depdata_one["LastPrice"     ]
    Volume         = depdata_one["Volume"        ]
    BidPrice1      = depdata_one["BidPrice1"     ]
    BidVolume1     = depdata_one["BidVolume1"    ]
    AskPrice1      = depdata_one["AskPrice1"     ]
    AskVolume1     = depdata_one["AskVolume1"    ]
    InstrumentID   = depdata_one["InstrumentID"  ]
    UpdateTime     = depdata_one["UpdateTime"    ]
    UpdateMillisec = depdata_one["UpdateMillisec"]
    if InstrumentID in InstrumentYYff:
        clear_timecolor(0)
        xx = BaseXX
        yy = InstrumentYYff[InstrumentID]
        colorLastPrice = "\033[32m"
        colorTime = "\033[47;30m"
        if AskPrice1 != "-" and LastPrice != "-" and BidPrice1 != "-" and (float(AskPrice1)-float(LastPrice)) <= (float(LastPrice)-float(BidPrice1)):
            colorLastPrice = "\033[31m"
        sys.stdout.write("\x1b7\x1b[%d;%df%s.%3s%s \033[m%s%11s\033[m %-6s\x1b8" % (yy,xx,UpdateTime,UpdateMillisec,colorTime,colorLastPrice,LastPrice,InstrumentID))
        sys.stdout.flush()
    if InstrumentID in InstrumentYYioC:
        clear_timecolor(33)
        xx = BaseXX+33
        yy = InstrumentYYioC[InstrumentID]
        colorLastPrice = "\033[32m"
        colorTime = "\033[47;30m"
        if AskPrice1 != "-" and LastPrice != "-" and BidPrice1 != "-" and (float(AskPrice1)-float(LastPrice)) <= (float(LastPrice)-float(BidPrice1)):
            colorLastPrice = "\033[31m"
        sys.stdout.write("\x1b7\x1b[%d;%df%s.%3s%s \033[m%s%11s\033[m %-13s\x1b8" % (yy,xx,UpdateTime,UpdateMillisec,colorTime,colorLastPrice,LastPrice,InstrumentID))
        sys.stdout.flush()
    if InstrumentID in InstrumentYYioP:
        clear_timecolor(73)
        xx = BaseXX+73
        yy = InstrumentYYioP[InstrumentID]
        colorLastPrice = "\033[32m"
        colorTime = "\033[47;30m"
        if AskPrice1 != "-" and LastPrice != "-" and BidPrice1 != "-" and (float(AskPrice1)-float(LastPrice)) <= (float(LastPrice)-float(BidPrice1)):
            colorLastPrice = "\033[31m"
        sys.stdout.write("\x1b7\x1b[%d;%df%s.%3s%s \033[m%s%11s\033[m %-13s\x1b8" % (yy,xx,UpdateTime,UpdateMillisec,colorTime,colorLastPrice,LastPrice,InstrumentID))
        sys.stdout.flush()

def clear_timecolor(ishift):
    if ishift == 0:
        yyitems = InstrumentYYff.values()
    if ishift == 33:
        yyitems = InstrumentYYioC.values()
    if ishift == 73:
        yyitems = InstrumentYYioP.values()
    for yyvalue in yyitems:
        sys.stdout.write("\x1b7\x1b[%d;%df\033[40;37m \033[m\x1b8" %(yyvalue, BaseXX+12+ishift))

def terminal_update_msg(YY, XX, msg):
    sys.stdout.write("\x1b7\x1b[%d;%df%-30s\x1b8" %(YY, XX, msg))
    sys.stdout.flush()

def terminal_update_act_info():
    # tqq16,ReqQryInvestorAccount()
    # trr39,OnRspQryInvestorAccount()
    if dnj_trade_common.InvestorAccount: # "{:,}".format(value)
        sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+11, BaseXX,    "", dnj_trade_common.InvestorAccount['r_datentime_micro']))
        sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+11, BaseXX+27, "PositionProfit:", dnj_trade_common.InvestorAccount['PositionProfit']))
        sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+11, BaseXX+57, "Available:", dnj_trade_common.InvestorAccount['Available']))
        sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+12, BaseXX+27, "Risk:", dnj_trade_common.InvestorAccount['Risk']))
        sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+12, BaseXX+57, "PreBalance:", dnj_trade_common.InvestorAccount['PreBalance']))
        sys.stdout.flush()

def terminal_update_connectivity():
    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+14, BaseXX,    "mrr11:", str(dnj['check']['isMrr11Coming'])))
#    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+15, BaseXX ,   "mrr11:", str(dnj_trade_common.rr_lastupdate['mrr11'])))
    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+14, BaseXX+27, "J:"    , str(dnj['check']['isJConnected'])))
#    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+15, BaseXX+27, "J:"    , str(dnj_trade_common.rr_lastupdate['in_irr'])))
    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+14, BaseXX+57, "F:"    , str(dnj['check']['isFConnected'])))
#    sys.stdout.write("\x1b7\x1b[%d;%df%s%s\x1b8" %(BaseYY+15, BaseXX+57, "F:"    , str(dnj_trade_common.rr_lastupdate['in_irr'])))
    sys.stdout.flush()


import sys, tty, termios
def _unix_getch():
    """Get a single character from stdin, Unix version"""
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())          # Raw read
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

def get_input_str(YY, XX):
    theListOfChars = []
    while True:
        char_this = _unix_getch()
        if ord(char_this) in [3,4]: #3 = Ctrl-C, #4 = Ctrl-D
            sys.exit()
        elif ord(char_this) == 127: #127 = backspace
            if len(theListOfChars) > 0:
                theListOfChars.pop()
        elif ord(char_this) == 13: #13 = enter
            return "".join(theListOfChars)
        else:
            theListOfChars.append(char_this)
        strInput = "cmd: " + "".join(theListOfChars)
        sys.stdout.write("\x1b7\x1b[%d;%df%-30s\x1b8" %(YY, XX, strInput))
        sys.stdout.flush()

def print_time():
    while (True):
        localtime = time.strftime("%H:%M:%S %Y/%m/%d", time.localtime(time.time()))
        sys.stdout.write("\0337\033[%d;%df\033[48;5;021m%s\033[0m\0338" % (30, 0, localtime))
        sys.stdout.flush()
        time.sleep(1)


if __name__ == '__main__':
    a = 5

