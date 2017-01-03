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

//json 헤더
#include <json/json.h>

//list
#include <list>

//sqllist3
#include <sqlite3.h>

//Cli_Sck 헤더 입포트
#include "Cli_Sck.hpp"

#define READ_BINARY 1
#define WRITE_BINARY 2


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
    void set_file_name(const string path);
    string file_name();
    
    //서버리스트
    void add_server_lst(const std::string s,
                        int prt);           //서버리스트 에 지정된 서버 ip insert
    void show_server_list();         //전송할 서버리스트 보여주기
    bool send_server_list();         //서버 리스트 로 전송
    list<Cli_Sck> server_list();
    
private:
    //파일을 파편화 하여 보관함
    bool _file_frag();      //파일 파편화
    bool _file_snd();       //파일 전송
   // bool _file_rcv();       //파일 수신 kevent 를 넣어야 하므로 나중에
    
private:
    
    
private:

    
    //보낼 서버 ip 리스트
    list<Cli_Sck> srv_lst_;
    string f_buff_;
    string f_name_;
    unsigned long long f_fme_siz_;
    list<Json::Value> f_frag_lst_;   //파일 파편화 리스트
    
    
    
    bool active_;
    
    
};

#endif /* Cuma_client_hpp */
