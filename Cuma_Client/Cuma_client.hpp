//
//  Cuma_client.hpp
//  Cuma_Client
//
//  Created by ace on 2016. 12. 30..
//  Copyright © 2016년 ace. All rights reserved.
//

#ifndef Cuma_client_hpp
#define Cuma_client_hpp


#include <iostream>

//fstream
#include <fstream>

//소켓 헤더
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <unistd.h>

//json 헤더
#include <json/json.h>

//list
#include <list>

//sqllist3
#include <sqlite3.h>


//프레임 헤더
#include "Cuma_Frame.hpp"

//Cli_Sck 헤더 입포트
#include "Cli_Sck.hpp"
#include "Error_Binary.hpp"
#include "C_Log.hpp"
#include <algorithm>


//데모이므로 한개의 파일만 여러 서버에 업로드가 가능함

using std::list;
using std::string;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;

//Cuma_Client는 파일을 파편화 시켜서 업로드 담당함
class Cuma_Client{
    
public:
    Cuma_Client();
    ~Cuma_Client();
    
    //전송시작
    void start();
    void stop();
    bool is_active();
    void set_active(bool b);
    
    //파일 선택
    bool set_file_name(const string path);
    string file_name();
    
    //서버리스트
    void add_server_lst(const std::string s,
                        int prt);           //서버리스트 에 지정된 서버 ip insert
    void show_server_list();            //전송할 서버리스트 보여주기
    bool send_to_server_list();         //서버 리스트 로 전송
    bool recv_from_server_list();       //서버리스트에서 수신
    
    list<Cli_Sck> server_list();
    
private:
    //파일을 파편화 하여 보관함
    bool _file_frag();      //파일 파편화
    bool _file_combine();   //파일 재조합
    bool _file_init_assay();//파일 분석
    
    
    bool _file_snd();       //파일 전송
    bool _file_rcv();       //파일 수신 kevent 를 넣어야 하므로 나중에
    
private:
    //로그 출력에 관한 부분
    void _CS_LOG(const string& s);
    void _CS_LOG(const string& s , unsigned long long siz);
    
private:
    //파일 네트워킹 부분
    bool _CS_SND( const int s, Json::Value& J);
    bool _CS_RCV( const int s, Json::Value& J);
    
    void _CS_INDI(unsigned long long f_siz, unsigned long long snd_siz);
    
    
private:
    
    //보낼 서버 ip 리스트
    list<Cli_Sck> srv_lst_;
    
    string f_buff_temp_;
    string f_name_;
    
    unsigned long long f_fme_siz_;
    list<Cuma_Frame> f_frag_lst_;      //파일 파편화 리스트
    
    list<Cuma_Frame> rcv_frag_lst_;     //파일 수신받은 파일 프레임 리스트
    
    
    Json::Value f_r_send_lst;           //전송한 파일 리스트 읽기
    Json::Value frame_save_lst;           //.Cuma파일 에서 읽은후 저장되어있는 파일 프레임 리스트
    bool active_;
    
    
};

#endif /* Cuma_client_hpp */
