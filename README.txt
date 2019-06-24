*** c_data 說明 ***

qq (api)

qq                          (in.txt/co.txt)     (co_irr.txt in_irr.txt, in 和 co 的 result)
cmd key:                    tcmd/mcmd/ccmd      tcmd/mcmd/ccmd (iRR)    
[ 0] q_datentime_micro      "2019/04/17 11:.."  "2019/04/17 11:.." (照抄) form cmd 的時間 q
[ 1] q_timestamp_micro      "1555470693618304"  "1555470693618304" (照抄) form cmd 的時間 q
[ 2] q_code                 "tqq01"             "tqq01"            (照抄)
[ 3] q_name                 "ReqUserLogin()"    "ReqUserLogin()"   (照抄)
[ 4] q_cmd (ccmd/tcmd/mcmd) "tcmd"              "tcmd"             (照抄)
[ 5] q_source (co/au/in)    "au"                "au"               (照抄)
[ 6] q_condition            "if005"             "if005"            (照抄)
[ 7] q_nRequestID           "470693618"         "470693618"        (照抄)
[ 8] f_iRR                  ""                  "0"                     
[ 9] (reserved)             (reserved)          (reserved)    
[10] (reserved)             (reserved)          (reserved)
[11] q_11               -> terminal mq send (before)               (照抄)
[12] q_12 (another log) -> terminal mq send (after) (another log) 
[13] q_13               -> server mq get (before) [n/a]
[14] q_14               -> server mq get (after)     
[15] q_15               -> server femas(before)      
[16] q_16               -> server femas(after)       
[17] q_17               -> server mq send (before)   
[18] q_18 (another log) -> server mq send (after)   (another log)   
[19] (reserved)             (reserved)          (reserved)
[20] from #21 data          XXXXXXXXXXXXX       XXXXXXXXXXXXXXXXXX (照抄)
                            XXXXXXXXXXXXX       XXXXXXXXXXXXXXXXXX (照抄)                                   
                            XXXXXXXXXXXXX       XXXXXXXXXXXXXXXXXX (照抄)                           

RSP/RTN (spi):
                            
rsp                         (rr_rsp.txt)        (rr_rsp.txt)
cmd key:                    trsp/mrsp/crsp      terr/merr/cerr (mrr08 trr08 crr08)                
[ 0] r_datentime_micro      "2019/04/17 11:.."  "2019/04/17 11:.."  RSP/RTN 被呼叫的第一時間        
[ 1] r_timestamp_micro      "1555470693618304"  "1555470693618304"  RSP/RTN 被呼叫的第一時間       
[ 2] r_code                 "trr35"             "trr08"          
[ 3] r_name                 "OnRspQryOrder()"   "OnRspError()"   
[ 4] r_cmd                  "trsp"              "merr"
[ 5] rsp_ErrorID            0                   23                                               
[ 6] rsp_ErrorMsg           "正确"              "錯誤"                                              
[ 7] r_nRequestID           "470000000"         "470000000"         (對 rsp 來說最重要的是 nRequestID)                                                 
[ 8] rsp_bIsLast            1                   1                              
[ 9] (reserved)             (reserved)          (reserved) 
[10] r_EMPTY                "" (EMPTY)          ""     
[11] r_11               -> server mq send (before)  
[12] r_12 (another log) -> server mq send (after)  (another log) 
[13] r_13               -> terminal mq get (before) 
[14] r_14               -> terminal mq get (after)  
[15] (reserved)             (reserved)          (reserved)      
[16] (reserved)             (reserved)          (reserved)      
[17] (reserved)             (reserved)          (reserved)      
[18] (reserved)             (reserved)          (reserved)      
[19] (reserved)             (reserved)          (reserved)     
[20] from #21 data          XXXXXXXXXXXXX       __END__                        
                            XXXXXXXXXXXXX                                         
                            XXXXXXXXXXXXX                                       

rtn:                        error                  
rtn:                        (rr_rtn.txt)         (rr_rtn.txt)       (rr_rtn.txt)        (rr_rtn.txt)       
rtn:                        terr (trr23 trr24)   trtn/mrtn          mrr12/mrr13:        mrr01/trr01/mrr03/trr03                                                                         
[ 0] r_datentime_micro      "2019/04/17 11:.."   "2019/04/17 11:.." "2019/04/17 11:.."  "2019/04/17 11:.."
[ 1] r_timestamp_micro      "1555470693618304"   "1555470693618304" "1555470693618304"  "1555470693618304"
[ 2] r_code                 "trr23"              "mrr11"            "mrr12"             "mrr03"           
[ 3] r_name                 "OnErrRtnOrde..()"   "OnRtnDepthMa..()" "OnRspSubMark..()"  "OnFrontDisco..()"   
[ 4] r_cmd                  "terr"               "mrtn"             "mrtn"              "mrtn"
[ 5] rsp_ErrorID            23                   ""                 ""                  ""                                            
[ 6] rsp_ErrorMsg           "錯誤"               ""                 ""                  ""                                                
[ 7] r_nRequestID           ""                   ""                 ""                  ""                                                           
[ 8] rsp_bIsLast            ""                   ""                 1 (rsp_bIsLast)     ""                              
[ 9] (reserved)             (reserved)          (reserved)          (reserved)          (reserved)
[10] r_EMPTY                (reserved)          (reserved)          (reserved)          (reserved)
[11] r_11               -> server mq send (before)      
[12] r_12 (another log) -> server mq send (after)  (another log)    
[13] r_13               -> terminal mq get (before)     
[14] r_14               -> terminal mq get (after)      
[15] (reserved)             (reserved)          (reserved)          
[16] (reserved)             (reserved)          (reserved)          
[17] (reserved)             (reserved)          (reserved)          
[18] (reserved)             (reserved)          (reserved)          
[19] (reserved)             (reserved)          (reserved)      
[20] from #21 data       XXXXXXXXXXXXX          XXXXXXXXXXXXX       XXXXXXXXXXXXX       XXXXXXXXXXXXX
                         XXXXXXXXXXXXX          XXXXXXXXXXXXX       XXXXXXXXXXXXX       XXXXXXXXXXXXX       
                         XXXXXXXXXXXXX          XXXXXXXXXXXXX       XXXXXXXXXXXXX       XXXXXXXXXXXXX
               
                         
P.S.
cmd: server (co/au) 的 cmd 在 co.txt
cmd: terminal python (in) 的 cmd 在 in.txt
irr: co.txt 和 in.txt 的 result 都在 irr.txt
rsp: 所有的 rsp 都在 rr_rsp.txt (除了 mrr12/mrr13 在 rr_rtn.txt) 
rtn: 所有的 trn 都在 rr_rtn.txt (除了 mrr11 獨立出來在 rr_rtn_mrr11.txt)
err: 所有的 OnErrRtnXXX 也都在 rr_rtn.txt
mrr01/trr01/mrr03/trr03 無法分類, 也在 rr_rtn.txt
rsp 的 EMPTY 就是那些會造成 Segmentation fault 的情形
所以目前只有 6 個檔案:
co.txt
in.txt
irr.txt
rr_rsp.txt
rr_rtn.txt
rr_rtn_mrr11.txt

P.S. qq timestamp_micro
terminal inint cmd       -> [ 1] q_timestamp_micro (cmd 產生的時間, 也許是 terminal/python)
terminal mq send (before)-> [11]
terminal mq send (after) -> [12](another log)
server mq get (before)   -> [13]
server mq get (after)    -> [14] 
server femas(before)     -> [15] j_timestamp_micro
server femas(after)      -> [15] f_timestamp_micro
server mq send (before)  -> [17]
server mq send (after)   -> [18] (another log)

P.S. rr timestamp_micro
server femas spi         -> [ 1] r_timestamp_micro (rr 產生的時間, from spi)
server mq send (before)  -> [11]
server mq send (after)   -> [12] (another log)
terminal mq get (before) -> [13]
terminal mq get (after)  -> [14]


處理的速度:
命令規定1秒只能20個, 所以1個命令 < 0.05 sec (50,000 micro sec) 即可
報價1秒2次. 假設要取得 300個合約 (IO*). 1/2/300 = 0.001666 sec (1,666 micro sec) 即可
IO1905: 64
IO*: 276

*** q_cmd/q_code/q_source/q_condition 說明 ***

q_cmd: 指令大類別 dnj_femas.out 自己的指令 ccmd, 和飛馬的 tcmd 及 mcmd

q_code: 指令的代號 tqq01, mqq01, cqq01
    (1) ccmd 類, 其實呼叫 console 的 function
        1.那些在 console.cpp 內的 
            tt: times                                                                                                   [0/1930]
            j: ttJstart
            c: cfg content
            f: filenamee
            s: status
            0: load_cfg_init()
            1: flip isTGoLogin
            2: flip isMGoLogin
            3: flip isTGoOrder
            4: flip isMGoSub
        2.以及一些些的 tcmd/mcmd (這個情形下, q_source: co 或 au)
            t1: tqq01: ReqUserLogin() 风控前置系统用户登录请求
            t2: tqq02: ReqUserLogout() 用户退出请求      
            m1: mqq01: ReqUserLogin() 风控前置系统用户登录请求      
            m2: mqq02: ReqUserLogout() 用户退出请求      
            m4: mqq04: SubMarketData() 订阅合约行情, 預定 IF* 和 IO*      
            m5: mqq05: UnSubMarketData() 退订合约行情, 預定 * 全部退訂 
    (2) tcmd (3) mcmd
        就是那一大堆 tqq, mqq (列表在 dnj_femas_api_input.cpp)
        
r_source: 指令的來源 
    au (dnj_femas.out 自動)
    co (console 手動下)
    in (from in.txt) 

r_condition: 程式自動 au 的那個位置下的指令 (if002), 如果是 console 手動下 固定 co001, 如果是 in.txt 固定是 in001
    "au"
        if000: (還沒有決定編號的 if, 例如 OnFrontConnected)
        if001: 程式開始後第一次 load_cfg_init(), 另外 main() 也是 if001 因為還無法寫 log 目錄
        if002: 每天凌晨 tt_now.date 不等於 dnj.ttloadCFG.date
        if003: isTConnectOK isTGoLogin 都 true, 但 isTLoginOK == false, 自動 T login (tqq01)
        if004: isMConnectOK isMGoLogin 都 true, 但 isMLoginOK == false, 自動 M login (mqq01)
        if005: T connect 之後, 自動登入
        if006: M connect 之後, 自動登入
        if007: M 登入成功後, 自動 sub
    "co"
        固定 co001 (寫在 console.cpp 的 *read_console_in pthread 內)
    "in"
        固定 in001 (寫在 python 內)


*** r_code 說明 ***
r_code: 
    定義在 dnj_femas_console.cpp 內的 crr (crr 一定是 rsp -- crsp)
    定義在 dnj_femas_spi_output.cpp 內的 trr 及 mrr
    
    
    
    
    