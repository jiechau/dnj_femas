/*


*/
#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // strcmp
#include <unistd.h> // sleep(1)
#include <iconv.h>  // iconv
#include <sys/stat.h> // mkdir
#include <sys/time.h> // gettimeofday
#include <fstream>
#include <iostream> // cin
#include <string>   // std::string

#include <dnj_femas_common.hpp>
#include <dnj_femas_console.hpp>




using namespace std;

extern dnj_femas_global_setting dnj;

//// dnj_femas_global_setting
// init static members

// id pwd addr
char *dnj_femas_global_setting::gBrokerID           = new char[50];
char *dnj_femas_global_setting::gUserID             = new char[50];
char *dnj_femas_global_setting::gInvestorID         = new char[50];
char *dnj_femas_global_setting::gPassword           = new char[50];
char *dnj_femas_global_setting::gExchangeID         = new char[50];
char *dnj_femas_global_setting::gClientID           = new char[50]; ///客户编码
char *dnj_femas_global_setting::gMAddr              = new char[100]; // 设置飞马平台服务的地址，可以注册多个地址备用
char *dnj_femas_global_setting::gTAddr              = new char[100]; // 设置飞马平台服务的地址，可以注册多个地址备用
// set up flags
int  dnj_femas_global_setting::iMP                   = 2;    // 1:file, 2:persistent
bool dnj_femas_global_setting::isTGoLogin            = true;
bool dnj_femas_global_setting::isMGoLogin            = true;
bool dnj_femas_global_setting::isTauLogin            = true;
bool dnj_femas_global_setting::isMauLogin            = true;
bool dnj_femas_global_setting::isTFakeGoLogin        = false;
bool dnj_femas_global_setting::isMFakeGoLogin        = false;
bool dnj_femas_global_setting::isTFakeConnectOK      = false;
bool dnj_femas_global_setting::isMFakeConnectOK      = false;
bool dnj_femas_global_setting::isTGoOrder            = true;
bool dnj_femas_global_setting::isMGoSub              = true;
// file names
char *dnj_femas_global_setting::basenamee              = new char[100];
char *dnj_femas_global_setting::datenamee              = new char[100];
char *dnj_femas_global_setting::pathnamee              = new char[100];
// c_data
char *dnj_femas_global_setting::filenamee_co           = new char[100]; // co.txt
char *dnj_femas_global_setting::filenamee_in           = new char[100]; // in.txt
char *dnj_femas_global_setting::filenamee_co_result    = new char[100]; // co_irr.txt
char *dnj_femas_global_setting::filenamee_in_result    = new char[100]; // in_irr.txt
char *dnj_femas_global_setting::filenamee_rr_rsp       = new char[100]; // rr_rsp.txt
char *dnj_femas_global_setting::filenamee_rr_rtn       = new char[100]; // rr_rtn.txt
char *dnj_femas_global_setting::filenamee_rr_rtn_mrr11 = new char[100]; // rr_rtn_mrr11.txt mrr11 OnRtnDepthMarketData() 深度行情通知
char *dnj_femas_global_setting::filenamee_rr_rtn_trr22 = new char[100]; // rr_rtn_trr22.txt trr22 OnRtnOrder() 报单回报及成交
char *dnj_femas_global_setting::filenamee_log          = new char[100]; 
//jtest
std::ofstream dnj_femas_global_setting::ofstream_co          ;
std::ofstream dnj_femas_global_setting::ofstream_co_result   ;
std::ofstream dnj_femas_global_setting::ofstream_in_result   ;
std::ofstream dnj_femas_global_setting::ofstream_rr_rsp      ;
std::ofstream dnj_femas_global_setting::ofstream_rr_rtn      ;
std::ofstream dnj_femas_global_setting::ofstream_rr_rtn_mrr11;
std::ofstream dnj_femas_global_setting::ofstream_rr_rtn_trr22;
std::ofstream dnj_femas_global_setting::ofstream_log;


// global status
bool   dnj_femas_global_setting::isJlogged             = false;
bool   dnj_femas_global_setting::isTConnectOK          = false;
bool   dnj_femas_global_setting::isMConnectOK          = false;
bool   dnj_femas_global_setting::isTLoginOK            = false;
bool   dnj_femas_global_setting::isMLoginOK            = false;
bool   dnj_femas_global_setting::isMReadContractsOK    = false;
// time
ttTime dnj_femas_global_setting::ttJstart           ;
ttTime dnj_femas_global_setting::ttTstart           ;
ttTime dnj_femas_global_setting::ttMstart           ;
ttTime dnj_femas_global_setting::ttloadCFG          ;
ttTime dnj_femas_global_setting::ttpEchotime        ; // pthread start time
ttTime dnj_femas_global_setting::ttpReadintxt       ; // pthread start time
ttTime dnj_femas_global_setting::ttpReadconsolein   ; // pthread start time
ttTime dnj_femas_global_setting::ttTconnect         ;
ttTime dnj_femas_global_setting::ttTlogin           ;
ttTime dnj_femas_global_setting::ttMconnect         ;
ttTime dnj_femas_global_setting::ttMlogin           ;
ttTime dnj_femas_global_setting::ttTdisconnect      ;
ttTime dnj_femas_global_setting::ttTlogout          ;
ttTime dnj_femas_global_setting::ttMdisconnect      ;
ttTime dnj_femas_global_setting::ttMlogout          ;

// api ptr
CUstpFtdcTraderApi* dnj_femas_global_setting::pTUserApi;
CUstpFtdcMduserApi* dnj_femas_global_setting::pMUserApi;
// pthread_t ptr
pthread_t* dnj_femas_global_setting::t_read_in_txt;

// static functions
// cqq001
int dnj_femas_global_setting::load_cfg_init(char* sSource_this, char* sCondition_this) {
    
    char cfg_filename[100] = "./dnj_femas.cfg";

    if (! check_file_existed (cfg_filename)) {
        printf("%s,cqq001,load_cfg_init(),%s,%s,\"%s\": file not exist!\n", get_currentTT().datentime_micro, sSource_this, sCondition_this, cfg_filename);
        return -1; // file not exist
    }
    
    char *cfg_lines[50];
    int icfg_cnt = 0;
    icfg_cnt = read_file (cfg_lines, cfg_filename);
    // printf("%d", icfg_idx); // counts
    char delim[5] = "=";
    char *cFields[10]; // counts of fields
    int idexFields = -1;
    for (int idx=0; idx<icfg_cnt; idx++) {
        // memset(cFields, 0, sizeof(cFields)); //c don't know why but this line 'cause the crash
        idexFields = my_split(cfg_lines[idx], delim, cFields);
        //printf(">%s<,>%s<\n", cFields[0], cFields[1]);
        if (strcmp(cFields[0], "basenamee"  ) == 0) strcpy(dnj.basenamee   ,cFields[1]);
        if (strcmp(cFields[0], "gBrokerID"  ) == 0) strcpy(dnj.gBrokerID   ,cFields[1]);
        if (strcmp(cFields[0], "gUserID"    ) == 0) strcpy(dnj.gUserID     ,cFields[1]);
        if (strcmp(cFields[0], "gInvestorID") == 0) strcpy(dnj.gInvestorID ,cFields[1]);
        if (strcmp(cFields[0], "gPassword"  ) == 0) strcpy(dnj.gPassword   ,cFields[1]);
        if (strcmp(cFields[0], "gExchangeID") == 0) strcpy(dnj.gExchangeID ,cFields[1]);
        if (strcmp(cFields[0], "gClientID"  ) == 0) strcpy(dnj.gClientID   ,cFields[1]);
        if (strcmp(cFields[0], "gMAddr"     ) == 0) strcpy(dnj.gMAddr      ,cFields[1]);
        if (strcmp(cFields[0], "gTAddr"     ) == 0) strcpy(dnj.gTAddr      ,cFields[1]);
    }


    // mark load cfg tt
    dnj.ttloadCFG = get_currentTT();
    // 這裡是 init global vars 
    // get global vars
    get_date8(dnj.ttloadCFG.tv, dnj.datenamee);
    //printf(">datenamee:%s<\n", dnj.datenamee);
    get_namee ();

    // mq=1
    // create or touch file needed
    create_file_dir ();
    // do a 'touch' file, reset to empty file everytime (so, do it only if it doesn't exist)
    create_file_touch ();

    // mq=2 ofstream (for append)

    dnj.ofstream_in_result.close();
    dnj.ofstream_co.close();
    dnj.ofstream_co_result.close(); 
    dnj.ofstream_rr_rsp.close(); 
    dnj.ofstream_rr_rtn.close();
    dnj.ofstream_rr_rtn_mrr11.close();    
    dnj.ofstream_rr_rtn_trr22.close();    
    dnj.ofstream_log.close(); 
 
    dnj.ofstream_in_result.open(dnj.filenamee_in_result, std::ios_base::app);
    dnj.ofstream_co.open(dnj.filenamee_co, std::ios_base::app);
    dnj.ofstream_co_result.open(dnj.filenamee_co_result, std::ios_base::app);
    dnj.ofstream_rr_rsp.open(dnj.filenamee_rr_rsp, std::ios_base::app);
    dnj.ofstream_rr_rtn.open(dnj.filenamee_rr_rtn, std::ios_base::app);
    dnj.ofstream_rr_rtn_mrr11.open(dnj.filenamee_rr_rtn_mrr11, std::ios_base::app);
    dnj.ofstream_rr_rtn_trr22.open(dnj.filenamee_rr_rtn_trr22, std::ios_base::app);
    dnj.ofstream_log.open(dnj.filenamee_log, std::ios_base::app);
    
    // log jstart
    // 這裡有點無奈, 因為沒有 load 之前是沒有 co.txt 的路徑, 所以只好 co.txt co_irr.txt 寫在一起
    // 所以 main() 的 co_irr.txt 會比 load_cfg_init() 的 co.txt 還要晚, 先這樣.
    if (dnj.isJlogged == false) {

//        char sline_dummy[5] = "--";
//        append_file(dnj.filenamee_co, sline_dummy);
//        append_file(dnj.filenamee_co_result, sline_dummy);

        ttTime tt_co = dnj.ttJstart;
        int iRequestID = atoi(tt_co.nRequestID);
        char *cFields_co[50];
        for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
        for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
        strcpy(cFields_co[ 0], tt_co.datentime_micro);
        strcpy(cFields_co[ 1], tt_co.timestamp_micro);
        strcpy(cFields_co[ 2], "cqq000");   
        strcpy(cFields_co[ 3], "main()");  
        strcpy(cFields_co[ 4], "ccmd"); 
        strcpy(cFields_co[ 5], sSource_this);   
        strcpy(cFields_co[ 6], sCondition_this);     
        strcpy(cFields_co[ 7], tt_co.nRequestID);        // 7, nRequestID
        strcpy(cFields_co[15], tt_co.timestamp_micro);   // 
        // 10times ""
//        for (int idmy=10; idmy<20; idmy++) strcpy(cFields_co[idmy], "");
        // data start from [#20], 21st fields 
        // 這裡沒有 data
        do_sline_co(cFields_co, 20);
        int iRR = 0; // 自己定義 
        ttTime tt_f_time = get_currentTT();
        sprintf(cFields_co[ 8],"%d", iRR);
        strcpy(cFields_co[16], tt_f_time.timestamp_micro);
        do_sline_co_result(cFields_co, 20);
        printf("%s,cqq000,main(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 

        dnj.isJlogged = true;
    }
    
    // log load cfg
    // 這裡有點無奈, 因為沒有 load 之前是沒有 co.txt 的路徑, 所以只好 co.txt co_result.txt 寫在一起
    ttTime tt_co = dnj.ttloadCFG;
    int iRequestID = atoi(tt_co.nRequestID);
    char *cFields_co[50];
    for (int idx=0; idx<50; idx++) cFields_co[idx] = new char[100];
    for (int idx=0; idx<50; idx++) strcpy(cFields_co[idx], "");
    strcpy(cFields_co[0], tt_co.datentime_micro);
    strcpy(cFields_co[1], tt_co.timestamp_micro);
    strcpy(cFields_co[2], "cqq001");   
    strcpy(cFields_co[3], "load_cfg_init()");  
    strcpy(cFields_co[4], "ccmd"); 
    strcpy(cFields_co[5], sSource_this);   
    strcpy(cFields_co[6], sCondition_this);     
    strcpy(cFields_co[ 7], tt_co.nRequestID);        // 7, nRequestID
    strcpy(cFields_co[15], tt_co.timestamp_micro);   // 
    // 10times ""
//    for (int idmy=10; idmy<20; idmy++) strcpy(cFields_co[idmy], "");
    // data start from [#20], 21st fields 
    strcpy(cFields_co[20], cfg_filename);
    do_sline_co(cFields_co, 21);
    int iRR = 0; // 自己定義 
    ttTime tt_f_time = get_currentTT();
    sprintf(cFields_co[8],"%d", iRR);
    strcpy(cFields_co[16], tt_f_time.timestamp_micro);
    do_sline_co_result(cFields_co, 21);
    printf("%s,cqq001,load_cfg_init(),%s,%s,iRR=[%d],nRequestID=[%d]\n", cFields_co[0], sSource_this, sCondition_this, iRR, iRequestID); 

    return 0;
}

void get_namee () { 
    sprintf(dnj.pathnamee,              "%s/%s",                  dnj.basenamee, dnj.datenamee); 
    sprintf(dnj.filenamee_co,           "%s/%s/co.txt",           dnj.basenamee, dnj.datenamee); // co.txt
    sprintf(dnj.filenamee_in,           "%s/%s/in.txt",           dnj.basenamee, dnj.datenamee); // in.txt
    sprintf(dnj.filenamee_co_result,    "%s/%s/co_irr.txt",       dnj.basenamee, dnj.datenamee); // co_irr.txt
    sprintf(dnj.filenamee_in_result,    "%s/%s/in_irr.txt",       dnj.basenamee, dnj.datenamee); // in_irr.txt
    sprintf(dnj.filenamee_rr_rsp,       "%s/%s/rr_rsp.txt",       dnj.basenamee, dnj.datenamee); // rr_rsp.txt
    sprintf(dnj.filenamee_rr_rtn,       "%s/%s/rr_rtn.txt",       dnj.basenamee, dnj.datenamee); // rr_rtn.txt
    sprintf(dnj.filenamee_rr_rtn_mrr11, "%s/%s/rr_rtn_mrr11.txt", dnj.basenamee, dnj.datenamee); // rr_rtn_mrr11.txt mrr11 OnRtnDepthMarketData() 深度行情通知
    sprintf(dnj.filenamee_rr_rtn_trr22, "%s/%s/rr_rtn_trr22.txt", dnj.basenamee, dnj.datenamee); // rr_rtn_trr22.txt trr22 OnRtnOrder() 报单回报及成交
    sprintf(dnj.filenamee_log,          "%s/%s/log.txt",          dnj.basenamee, dnj.datenamee); // rr_rtn_mrr11.txt mrr11 OnRtnDepthMarketData() 深度行情通知
}
    
void create_file_dir () {
    // pathnamee
    mkdir(dnj.pathnamee, 0755);
    mkdir("./condatat", 0755);
    mkdir("./condatam", 0755);
}
void create_file_touch () {
    if (! check_file_existed (dnj.filenamee_in)) {
        fstream fs;
        fs.open(dnj.filenamee_in, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_in_result)) {
        fstream fs;
        fs.open(dnj.filenamee_in_result, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_co)) {
        fstream fs;
        fs.open(dnj.filenamee_co, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_co_result)) {
        fstream fs;
        fs.open(dnj.filenamee_co_result, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_rr_rsp)) {
        fstream fs;
        fs.open(dnj.filenamee_rr_rsp, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_rr_rtn)) {
        fstream fs;
        fs.open(dnj.filenamee_rr_rtn, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_rr_rtn_mrr11)) {
        fstream fs;
        fs.open(dnj.filenamee_rr_rtn_mrr11, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_rr_rtn_trr22)) {
        fstream fs;
        fs.open(dnj.filenamee_rr_rtn_trr22, ios::out);
        fs.close();
    }
    if (! check_file_existed (dnj.filenamee_log)) {
        fstream fs;
        fs.open(dnj.filenamee_log, ios::out);
        fs.close();
    }
}


// 執行 in.txt 的 pthread
void *read_in_txt(void* data) {

    dnj.ttpReadintxt = get_currentTT();
    // ?? logt to co.txt co_result.txt
    
    char p_command[100];
    sprintf(p_command, "tail -n1 -f %s 2>&1", dnj.filenamee_in);
    FILE* pipe = popen(p_command, "r");
    if (!pipe)
    {
        printf("popen error!\n");
        return 0;
    }
    char cmd_in[300];
    char *cFields_in[100];
    char *pos;
    ttTime tt_stdin;
    while (true) {
        
        memset(&cmd_in, 0, sizeof(cmd_in));
        //memset(&cFields_in, 0, sizeof(cFields_in));  //c
        
        fgets(cmd_in, 300, pipe);
        // trim newline
        if ((pos=strchr(cmd_in, '\n')) != NULL) *pos = '\0';
        //printf("%s\n", cmd_in);
        // split (also do the dummy "~|~")
        read_in_split(cFields_in, cmd_in);
        
        // 這裡可以記錄一次取得的時間 server mq get (after)
        tt_stdin = get_currentTT();
        //strcpy(cFields_in[13], tt_stdin.timestamp_micro); // old, will be deleted
        strcpy(cFields_in[14], tt_stdin.timestamp_micro);
      
        // send to api
        if ((strcmp(cFields_in[4], "tcmd") == 0) ||
           (strcmp(cFields_in[4], "mcmd") == 0)) {
            // do_api_in(cFields_in, pTUserApi, pMUserApi);
            do_api_in(cFields_in); // 這個情形, 固定 , cFields_in[5]="in", cFields_in[6]="in001" (寫在 python 內)
        }
        
        // if (strcmp(cFields_in[2], "jcmd") == 0) {}

    }
    pclose(pipe);

  //pthread_exit(NULL); // 離開子執行緒
}

/// RSP/RTN (spi):
void send_sline_spi_output(char* cFields[], int icnt) {
    //server mq send (before)  -> [r_11]
    ttTime tt_11 = get_currentTT();
    strcpy(cFields[11], tt_11.timestamp_micro);
    // result
    char sline_result[300];
    sprintf(sline_result,   "%s", cFields[0]); 
    for (int idx=1; idx<icnt; idx++) {
        sprintf(sline_result,"%s,%s", sline_result, cFields[idx]);
    }
    // 這裡要分別 filename
    char filenamee_this[100];
    get_filenamee_value(filenamee_this, cFields[2]);
    if (dnj.iMP == 1) append_file(filenamee_this, sline_result);
    if (dnj.iMP == 2) append_file_persistent(filenamee_this, sline_result);
    // server mq send (after)  -> [r_12]
    // send to log (add time cFields[12])
}

/// qq (api)
void send_sline_in_result(char* cFields[], int icnt) {
    //server mq send (before)  -> [17] // q_17
    ttTime tt_17 = get_currentTT();
    strcpy(cFields[17], tt_17.timestamp_micro);
    // result
    char sline_result[300];
    sprintf(sline_result,   "%s", cFields[0]); 
    for (int idx=1; idx<icnt; idx++) {
        sprintf(sline_result,"%s,%s", sline_result, cFields[idx]);
    }
    // 這裡要分別 filename
    char filenamee_this[100];
    get_filenamee_value(filenamee_this, cFields[2]);
    if (dnj.iMP == 1) append_file(filenamee_this, sline_result);
    if (dnj.iMP == 2) append_file_persistent(filenamee_this, sline_result);
    //server mq send (after)  -> [18] // q_18
    ttTime tt_18 = get_currentTT();
    strcpy(cFields[18], tt_18.timestamp_micro);
    memset(sline_result, 0, sizeof(sline_result));
    sprintf(sline_result,   "%s", cFields[0]); 
    for (int idx=1; idx<icnt; idx++) {
        sprintf(sline_result,"%s,%s", sline_result, cFields[idx]);
    }
    // send to log (add time cFields[18]) 
    append_file_persistent(dnj.filenamee_log, sline_result);
}
void append_file (char* filenamee_this, char* sline_this) {
	std::ofstream outfile;
	outfile.open(filenamee_this, std::ios_base::app);
	outfile << sline_this << std::endl; 
}
void append_file_persistent (char* filenamee_this, char* sline_this) {
	//std::ofstream outfile;
	//outfile.open(filenamee_this, std::ios_base::app);
    if (strcmp(dnj.filenamee_rr_rsp, filenamee_this) == 0) {
        dnj.ofstream_rr_rsp << sline_this << std::endl; 
        dnj.ofstream_rr_rsp.flush();
    }
    if (strcmp(dnj.filenamee_rr_rtn_mrr11, filenamee_this) == 0) {
        dnj.ofstream_rr_rtn_mrr11 << sline_this << std::endl; 
        dnj.ofstream_rr_rtn_mrr11.flush();
    }
    if (strcmp(dnj.filenamee_rr_rtn_trr22, filenamee_this) == 0) {
        dnj.ofstream_rr_rtn_trr22 << sline_this << std::endl; 
        dnj.ofstream_rr_rtn_trr22.flush();
    }
    if (strcmp(dnj.filenamee_log, filenamee_this) == 0) {
        dnj.ofstream_log << sline_this << std::endl; 
        dnj.ofstream_log.flush();
    }
    if (strcmp(dnj.filenamee_co, filenamee_this) == 0) {
        dnj.ofstream_co << sline_this << std::endl; 
        dnj.ofstream_co.flush();
    }
    if (strcmp(dnj.filenamee_in_result, filenamee_this) == 0) {
        dnj.ofstream_in_result << sline_this << std::endl; 
        dnj.ofstream_in_result.flush();
    }
    if (strcmp(dnj.filenamee_co_result, filenamee_this) == 0) {
        dnj.ofstream_co_result << sline_this << std::endl; 
        dnj.ofstream_co_result.flush();
    }
    if (strcmp(dnj.filenamee_rr_rtn, filenamee_this) == 0) {
        dnj.ofstream_rr_rtn << sline_this << std::endl; 
        dnj.ofstream_rr_rtn.flush();
    }
}

void get_filenamee_value(char *filenamee_this, char *f_code) {
    // although its kinda stupid...

    //??? 這裡要決定, 要不要多一個參數, sSource
    //??? 要決定 in 的 qq/rr 要不要寫在一起. (if so, 欠的時間 要補回去)
    //??? send_sline_co, send_sline_co_result
    
    // rsp
    if(strcmp(f_code, "trr08") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr09") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr10") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr12") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr13") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr35") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr36") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr37") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr38") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr39") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr40") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "trr42") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "mrr08") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "mrr09") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    if(strcmp(f_code, "mrr10") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rsp       );
    // rtn
    if(strcmp(f_code, "trr23") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr24") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr21") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr25") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr30") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr01") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr03") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr01") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "trr03") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr01") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr03") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr12") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    if(strcmp(f_code, "mrr13") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn       );
    // trr22
    if(strcmp(f_code, "trr22") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn_trr22 );
    // mrr11
    if(strcmp(f_code, "mrr11") == 0) strcpy(filenamee_this, dnj.filenamee_rr_rtn_mrr11 );
    // qq
    if(strcmp(f_code, "tqq04") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq05") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq12") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq13") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq14") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq15") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq16") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq17") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq19") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq22") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    if(strcmp(f_code, "tqq29") == 0) strcpy(filenamee_this, dnj.filenamee_in_result    );
    //if(strcmp(f_code, "xxx11") == 0) strcpy(filenamee_this, dnj.filenamee_co           );
    //if(strcmp(f_code, "xxx11") == 0) strcpy(filenamee_this, dnj.filenamee_in           );
    //if(strcmp(f_code, "xxx11") == 0) strcpy(filenamee_this, dnj.filenamee_co_result    );
}

void read_in_split(char *cFields[], char * cmd) {

    // do the dummy "~|~" replace
	//std::string data = "tcmd,中文,,tqq02,ReqUserLogout(),421920946,,,,,0059,,990407,,";
    std::string data(cmd); // char * to std:string
        //std::cout<<data<<std::endl;
	findAndReplaceAll(data, ",,", ",~|~,");
        //std::cout<<data<<std::endl;
    const char *cstr = data.c_str();   //  std:string to char *
    strcpy(cmd, cstr);                 //  std:string to char *
    
    // strtok_r split
    char delim[5] = ",";
    int idexFields = -1;
    idexFields = my_split(cmd, delim, cFields);
    
    // replace "~|~" back to null
    for (int idx=0; idx<=idexFields; idx++) {
        if (strcmp(cFields[idx], "~|~") == 0) {
            strcpy(cFields[idx], "");
        }
        //printf("out: cFields[%d]:%s\n", idx, cFields[idx]);
    }
}


// everything common

int my_split(char *cmd, char *delim, char* cFields[]) {
    char* pch = NULL;
    char* pSave = NULL;
    pch = strtok_r(cmd, delim, &pSave);
    int idexFields = -1;
    while(pch){
        idexFields++;
        cFields[idexFields] = new char[100];
        sprintf(cFields[idexFields], pch);
        //printf("in: cFields[%d]:%s\n", idexFields, cFields[idexFields]);
        pch = strtok_r(NULL, delim, &pSave);
    }
    return idexFields;
}

// 不能傳入空字串, 會回傳上一次的結果
void my_iconv(const char * from_charset, const char * to_charset, char * inptr, char * outptr) {
    // 不能傳入空字串, 會回傳上一次的結果
    if (strlen(inptr) == 0) {
        strcpy(outptr, "");
        return;
    }
    size_t inleft = strlen(inptr);
    size_t outleft = inleft*4;
    iconv_t cd; /* conversion descriptor */
    if ((cd = iconv_open(to_charset, from_charset)) == (iconv_t)(-1)) {
        fprintf(stderr, "Cannot open converter from %s to %sn", from_charset, to_charset);
        //exit(8);
    }
    /* return code of iconv() */
    int rc = iconv(cd, & inptr, & inleft, & outptr, & outleft);
    if (rc == -1) {
        fprintf(stderr, "Error in converting characters\n");
        if (errno == E2BIG)
            printf("errno == E2BIG\n");
        if (errno == EILSEQ)
            printf("errno == EILSEQ\n");
        if (errno == EINVAL)
            printf("errno == EINVAL\n");
        iconv_close(cd);
        //exit(8);
    }
    iconv_close(cd);
}

ttTime get_currentTT() {
    ttTime tt_this;
    //timeval tv;
    tt_this.tv = get_current_timeval();
    //char timestamp[50];       //   1553826774          tv.tv_sec
    get_timestamp(tt_this.tv, tt_this.timestamp);
    //char timestamp_micro[50]; //   1553826774366354    "%d%06d", tv.tv_sec, tv.tv_usec
    get_timestamp_micro(tt_this.tv, tt_this.timestamp_micro);
    //char nRequestID[50];      //   1553[826774366]354
    get_nRequestID(tt_this.timestamp_micro, tt_this.nRequestID);
    //char date[50];            //  "2019/04/01"
    get_date(tt_this.tv, tt_this.date);
    //char date8[50];            //  "20190401"
    get_date8(tt_this.tv, tt_this.date8);
    //char time[50];            //  "09:16:30"
    get_time(tt_this.tv, tt_this.time);
    //char datentime[50];       //  "2019/04/01 09:16:30"
    get_datentime(tt_this.tv, tt_this.datentime);
    //char datentime_micro[50]; //  "2019/04/01 09:16:30 414790"
    get_datentime_micro(tt_this.tv, tt_this.datentime_micro);
    return tt_this;
}

timeval get_current_timeval () {
    // 取得目前 timestamp 
    //struct timeval start;
    timeval curTime_t;
    gettimeofday(&curTime_t, NULL);
    return curTime_t;
}
void get_date (timeval curTime_t, char* date_this) {
    char currentTime_t[30];
    strftime(currentTime_t, 30, "%Y/%m/%d", localtime(&curTime_t.tv_sec));
    strcpy(date_this, currentTime_t);
}
void get_date8 (timeval curTime_t, char* date_this) {
    char currentTime_t[30];
    strftime(currentTime_t, 30, "%Y%m%d", localtime(&curTime_t.tv_sec));
    strcpy(date_this, currentTime_t);
}
void get_time (timeval curTime_t, char* time_this) {
    char currentTime_t[30];
    strftime(currentTime_t, 30, "%H:%M:%S", localtime(&curTime_t.tv_sec));
    strcpy(time_this, currentTime_t);
}
void get_datentime (timeval curTime_t, char* datentime_this) {
    char currentTime_t[30];
    strftime(currentTime_t, 30, "%Y/%m/%d %H:%M:%S", localtime(&curTime_t.tv_sec));
    strcpy(datentime_this, currentTime_t);
}
void get_datentime_micro (timeval curTime_t, char* datentime_micro_this) {
    char currentTime_t[30];
    char currentTime_m[30];
    strftime(currentTime_t, 30, "%Y/%m/%d %H:%M:%S", localtime(&curTime_t.tv_sec));
    sprintf(currentTime_m, "%s %06d", currentTime_t, curTime_t.tv_usec);
    strcpy(datentime_micro_this, currentTime_m);
}
void get_nRequestID (char* datentime_micro_this, char* nRequestID_this) {
    //   1553[826774366]354
    memcpy(nRequestID_this, &datentime_micro_this[4], 9);
    nRequestID_this[9] = '\0';
}
void get_timestamp (timeval curTime_t, char* timestamp_this) {
    sprintf(timestamp_this, "%d", curTime_t.tv_sec);
}
void get_timestamp_micro(timeval curTime_t, char* timestamp_micro_this) {
    sprintf(timestamp_micro_this, "%d%06d", curTime_t.tv_sec, curTime_t.tv_usec);
}


// 那個顯示時間, 只是為了 keep connection
void *echo_time(void* data) {
    dnj.ttpEchotime = get_currentTT();
    ttTime tt_echo;
	while (1) {
        tt_echo = get_currentTT();
		printf("\0337\033[30;60f\033[48;5;021mp %s\033[0m\0338", tt_echo.datentime_micro);
        //printf("\0337\033[30;60f\033[48;5;021mp %s\033[0m\0338", dnj.gBrokerID);
		fflush(stdout);
		sleep(1);
	}
  //pthread_exit(NULL); // 離開子執行緒
}

// check file exist
bool check_file_existed (char* filename_this) {
    struct stat buffer;   
    return (stat (filename_this, &buffer) == 0); 
}

void read_stdin (char *cmd_this) {
	cin.getline(cmd_this, 300);	
}

int read_file (char *lines_this[], char *file_to_read) {
    int idx = 0;
    string line_this;
    ifstream myfile (file_to_read, ios::in);
    if (myfile.is_open())
    {
        while ( getline (myfile, line_this) )
        {
            lines_this[idx]  = new char[100]; // string length "IO1903-C-3000"
            sprintf(lines_this[idx], "%.100s", line_this.c_str());
            idx++;
        }
        myfile.close();
    }
    return idx; // counts
}

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size() - 1);
        //pos =data.find(toSearch, pos + replaceStr.size());
	}
}

