//
//  Cli_Sck.cpp
//  Cuma_Client
//
//  Created by ace on 2017. 1. 1..
//  Copyright © 2017년 ace. All rights reserved.
//

#include "Cli_Sck.hpp"


Cli_Sck::~Cli_Sck(){
    
}

//소켓 프로퍼티 영역
const int Cli_Sck::srv_sck(){
    return srv_sock_;
}
const char* Cli_Sck::srv_addr(){
    return srv_addr_;
}
const int Cli_Sck::srv_prt(){
    return srv_prt_;
}
const int Cli_Sck::srv_lstn(){
    return srv_lstn_;
}
 sockaddr_in Cli_Sck::srv_sck_addr(){
    return srv_sck_addr_;
}


//소켓 set 영역
void Cli_Sck::set_sock(int d){
    srv_sock_ = d;
}
void Cli_Sck::set_prt(int p){
    srv_prt_ = p;
}
void Cli_Sck::set_addr(const char* a){
    memcpy(srv_addr_, a, strlen(a));
}

void Cli_Sck::set_lst(int s){
    srv_lstn_ = s;
}




void Cli_Sck::set_sck_addr(){
    
    srv_sck_addr_.sin_family = AF_INET;
    srv_sck_addr_.sin_addr.s_addr = inet_addr(srv_addr_);
    srv_sck_addr_.sin_port = htons(srv_prt_);
    
}
