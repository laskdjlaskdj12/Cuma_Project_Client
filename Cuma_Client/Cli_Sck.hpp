//
//  Cli_Sck.hpp
//  Cuma_Client
//
//  Created by ace on 2017. 1. 1..
//  Copyright © 2017년 ace. All rights reserved.
//

#ifndef Cli_Sck_hpp
#define Cli_Sck_hpp
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/types.h>


class Cli_Sck{
public:
    Cli_Sck():srv_sock_(0),srv_prt_(0),srv_lstn_(0),active_(false){};
    ~Cli_Sck();
    
    //소켓 프로퍼티 영역
    const int srv_sck();            //소켓 디스크립터
    const char* srv_addr();         //서버 ip
    const int srv_prt();            //서버 prt
    const int srv_lstn();           //클라이언트 listen 크기
     sockaddr_in srv_sck_addr();
    
    
    //소켓 set 영역
    void set_sock(int d);         //소켓 디스크립터
    void set_prt(int p);          //서버 prt
    void set_addr(const char* a);//서버 ip
    void set_lst(int s);          //클라이언트 listen 크기
    
    void set_sck_addr();
    
    
private:
    int srv_sock_;                   //서버 소켓
    int srv_prt_;                    //서버 포트
    char* srv_addr_;                  //서버 주소
    int srv_lstn_;                 //클라이언트 서버 대기 큐
    bool active_;                    //서버가 시작되었는지
    
    sockaddr_in srv_sck_addr_;  //서버 sock_addr
};

#endif /* Cli_Sck_hpp */
