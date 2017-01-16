//
//  Cuma_Frame.cpp
//  Cuma_Client
//
//  Created by ace on 2017. 1. 15..
//  Copyright © 2017년 ace. All rights reserved.
//

#include "Cuma_Frame.hpp"


Cuma_Frame::Cuma_Frame():fr_siz_(0),fr_MOD_(0),fr_prt_(0),fr_num_(0){
    fr_addr_ = new char[24];
}
Cuma_Frame::Cuma_Frame(string fr_n,
                       unsigned int fr_num,
                       unsigned long long fr_siz):fr_siz_(0),fr_MOD_(0),fr_prt_(0),fr_num_(0){
    fr_name_ = fr_n;
    fr_num_ = fr_num;
    fr_siz_ = fr_siz;
    
}
Cuma_Frame::~Cuma_Frame(){}

unsigned long long Cuma_Frame::fr_siz() { return fr_siz_; }
string Cuma_Frame::fr_binary()          { return fr_binary_; }
string Cuma_Frame::fr_name()            { return fr_name_; }
string Cuma_Frame::f_name()             { return f_name_; }
unsigned int Cuma_Frame::fr_num()       { return fr_num_; }
Json::Value  Cuma_Frame::fr_json()      { return fr_json_;}
int          Cuma_Frame::fr_MOD()       { return fr_MOD_; }
string       Cuma_Frame::fr_addr()      { return fr_addr_;}
int          Cuma_Frame::fr_prt()       { return fr_prt_; }



void  Cuma_Frame::set_fr_siz(unsigned long long l)  { fr_siz_ = l;  }
void  Cuma_Frame::set_fr_binary(string s)           { fr_binary_ = s; }
void  Cuma_Frame::set_fr_name(string s)             { fr_name_ = s; }
void  Cuma_Frame::set_f_name(string s)              { f_name_ = s;  }
void  Cuma_Frame::set_fr_num(unsigned int num)      { fr_num_ = num;}
void Cuma_Frame::set_fr_json(Json::Value j)         { fr_json_ = j; }
void Cuma_Frame::set_fr_MOD(int n)                  { fr_MOD_ = n;  }
void  Cuma_Frame::set_fr_addr(string s)             { fr_addr_ = s; }
void  Cuma_Frame::set_fr_prt(int p)                 { fr_prt_  = p; }


Json::Value Cuma_Frame::to_json(){
    Json::Value root;
    root["F_siz"] =         (Json::UInt64)fr_siz_;
    root["F_name"] =        f_name_;
    root["F_frame_num"] =   fr_num_;
    root["F_frame_name"]=   fr_name_;
    root["F_binary"] =      fr_binary_;
    root["MODE"] =          fr_MOD_;
    root["F_addr"] =        fr_addr_;
    root["F_port"] =        fr_prt_;
    return root;
}


void Cuma_Frame::json_to_mem(){
    f_name_ =       fr_json_["F_name"].asString();
    fr_siz_ =       fr_json_["F_siz"].asUInt64();
    fr_num_ =       fr_json_["F_frame_num"].asUInt();
    fr_binary_ =    fr_json_["F_binary"].asString();
    fr_addr_ =      fr_json_["F_addr"].asString();
    fr_prt_ =       fr_json_["F_port"].asInt();
    fr_MOD_ =       fr_json_["MODE"].asInt();
}
