/*


*/
#include <string>   // std::string

#include "USTPFtdcTraderApi.h"
#include "USTPFtdcMduserApi.h"

#include <dnj_femas_api_input.hpp>

// time struct
struct ttTime
{
    timeval tv;
    //struct timeval {
    //  time_t      tv_sec;   //seconds
    //  suseconds_t tv_usec;  //microseconds
    //};
    char timestamp[30];       //   1553826774          tv.tv_sec
    char timestamp_micro[30]; //   1553826774366354    "%d%06d", tv.tv_sec, tv.tv_usec
    char nRequestID[30];      //   1553[826774366]354
    char date[30];            //  "2019/04/01"
    char date8[30];           //  "20190401"
    char time[30];            //  "09:16:30"
    char datentime[30];       //  "2019/04/01 09:16:30"
    char datentime_micro[30]; //  "2019/04/01 09:16:30 414790"
};

// dnj_femas_global_setting
class dnj_femas_global_setting {
public:
    dnj_femas_global_setting(){};
    ~dnj_femas_global_setting(){};
    static int RequestCount;
    // id pwd addr
    static char * gBrokerID   ;
    static char * gUserID     ;
    static char * gInvestorID ;
    static char * gPassword   ;
    static char * gExchangeID ;
    static char * gClientID   ; ///客户编码
	static char * gMAddr;       // 设置飞马平台服务的地址，可以注册多个地址备用
	static char * gTAddr;       // 设置飞马平台服务的地址，可以注册多个地址备用
    // set up flags
    static int  iMP;
    static bool isTGoLogin;
    static bool isMGoLogin;
    static bool isTauLogin;
    static bool isMauLogin;
    static bool isTFakeGoLogin;
    static bool isMFakeGoLogin;
    static bool isTFakeConnectOK;
    static bool isMFakeConnectOK;
    static bool isTGoOrder;
    static bool isMGoSub;
    // file names
    static char *  basenamee              ; // /home/jie/c_data
    static char *  datenamee              ; // 20190403
    static char *  pathnamee              ; // /home/jie/c_data/20190403
    static char *  filenamee_co           ; // co.txt
    static char *  filenamee_in           ; // in.txt
    static char *  filenamee_co_result    ; // co_irr.txt
    static char *  filenamee_in_result    ; // in_irr.txt
    static char *  filenamee_rr_rsp       ; // rr_rsp.txt
    static char *  filenamee_rr_rtn       ; // rr_rtn.txt
    static char *  filenamee_rr_rtn_mrr11 ; // rr_rtn_mrr11.txt mrr11 OnRtnDepthMarketData() 深度行情通知
    static char *  filenamee_rr_rtn_trr22 ; // rr_rtn_trr22.txt trr22 OnRtnOrder() 报单回报及成交
    static char *  filenamee_log          ;
    //jtest
    // std::ofstream outfile_irr; // for output append
    static std::ofstream ofstream_co          ;
    static std::ofstream ofstream_co_result   ;
    static std::ofstream ofstream_in_result   ;
    static std::ofstream ofstream_rr_rsp      ;
    static std::ofstream ofstream_rr_rtn      ;
    static std::ofstream ofstream_rr_rtn_mrr11;
    static std::ofstream ofstream_rr_rtn_trr22;
    static std::ofstream ofstream_log         ;
    
    // global status
    static bool isJlogged;
    static bool isTConnectOK;
    static bool isMConnectOK;
    static bool isTLoginOK;
    static bool isMLoginOK;
    static bool isMReadContractsOK;
    // time
    static ttTime ttJstart          ; // server start
    static ttTime ttloadCFG         ; // load CFG
    static ttTime ttTstart          ; 
    static ttTime ttMstart          ;
    static ttTime ttpEchotime       ;
    static ttTime ttpReadintxt      ;
    static ttTime ttpReadconsolein  ;
    static ttTime ttTconnect        ;
    static ttTime ttTlogin          ;
    static ttTime ttMconnect        ;
    static ttTime ttMlogin          ;
    static ttTime ttTdisconnect     ;
    static ttTime ttTlogout         ;
    static ttTime ttMdisconnect     ;
    static ttTime ttMlogout         ;
    // api ptr
    static CUstpFtdcTraderApi* pTUserApi;
    static CUstpFtdcMduserApi* pMUserApi;
    // pthread_t ptr
    static pthread_t *t_read_in_txt;
    // functions
    static int getRequestCount(); // test 
    static int load_cfg_init(char* sSource_this, char* sCondition_this);
    static void set_datentime_jstart();
    static char* get_datentime_jstart();
    static void set_datentime_tstart();
    static char* get_datentime_tstart();
    static void set_datentime_mstart();
    static char* get_datentime_mstart();
};

// femas depend
//void get_datenamee ();
void get_namee ();
void create_file_dir();
void create_file_touch();
void *read_in_txt(void*);
void read_in_split(char *cFields[], char * cmd);
void send_sline_spi_output(char* cFields[], int icnt);
void send_sline_in_result(char* cFields[], int icnt);
//void do_sline_in_result(char* cFields[], int icnt); // mq=1
//void do_sline_in_result_persistent(char* cFields[], int icnt); // mq=2, //jtest
void append_file (char* filenamee_this, char* sline_this); // mq=1
void append_file_persistent (char* filenamee_this, char* sline_this); // mq=2
void get_filenamee_value(char *filenamee_this, char *f_code);

// everything common
int my_split(char *cmd, char *delim, char* cFields[]);
void my_iconv(const char * from_charset, const char * to_charset, char * inptr, char * outptr);

ttTime get_currentTT();
timeval get_current_timeval();
void get_date (timeval curTime_t, char* date_this);
void get_date8(timeval curTime_t, char* date_this);
void get_time (timeval curTime_t, char* time_this);
void get_datentime (timeval curTime_t, char*);
void get_datentime_micro (timeval curTime_t, char*);
void get_nRequestID (char* datentime_micro_this, char* nRequestID_this);
void get_timestamp (timeval curTime_t, char*);
void get_timestamp_micro(timeval curTime_t, char*);

void *echo_time(void*);
bool check_file_existed (char* filename_this);
void read_stdin (char *);
int read_file (char *lines_this[], char *file_to_read);
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr);

