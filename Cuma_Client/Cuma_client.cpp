//
//  Cuma_client.cpp
//  Cuma_Client
//
//  Created by ace on 2016. 12. 30..
//  Copyright © 2016년 ace. All rights reserved.
//

#include "Cuma_client.hpp"


Cuma_Client::Cuma_Client(){
    srv_lst_.clear();
    f_frag_lst_.clear();
    f_name_.clear();
    f_buff_.clear();
}



Cuma_Client::~Cuma_Client(){
    srv_lst_.clear();
    f_frag_lst_.clear();
    f_name_.clear();
    f_buff_.clear();
}



//전송시작
void Cuma_Client::start(){
    active_ = true;
}


void Cuma_Client::stop(){
    active_ = false;
}


bool Cuma_Client::is_active(){
    return active_;
}


void Cuma_Client::set_active(bool b){
    active_ = b;
}



//파일 선택
void Cuma_Client::set_file_name(const string path){
    
    ifstream o_file(path,std::ios::binary);
    
    if(o_file.is_open() != true){
        
        std::cout<<"[Info] : Wrong Path"<<std::endl;
        
        return;
    }
    
    f_name_ = path;
    
}


string Cuma_Client::file_name(){
    return f_name_;
}



//서버리스트에 지정된 서버 ip insert
void Cuma_Client::add_server_lst(const std::string s,
                                 int prt){
    
    Cli_Sck s_temp;
    s_temp.set_addr(s.c_str());
    s_temp.set_prt(prt);
    s_temp.set_sck_addr();
    srv_lst_.push_back(s_temp);
}



//전송할 서버리스트 보여주기
void Cuma_Client::show_server_list(){
    std::cout<<"=========== [Server_Info] ============"<<std::endl;
    
    for(std::list<Cli_Sck>::iterator it = srv_lst_.begin();
        it != srv_lst_.end();
        it++){
        
        std::cout<<"["<<(*it).srv_addr()<<":"<<(*it).srv_prt()<<"]"<<std::endl;
    }
}



//서버 리스트 로 파일 파편화 전송
bool Cuma_Client::send_server_list(){
    
    /*std::cout<<"[Info] : File Fragmant has "<<f_frag_lst_.size()<<" frags."<<std::endl;
     
     std::cout<<"[Info] : Each File_Fragment frame is : "<<(f_buff_.size()/srv_lst_.size())<<" byte."<<std::endl;*/
    
    //파일 파편화 시작
    _file_frag();
    
    //파일 전송 시작
    _file_snd();
    
    return true;
}

list<Cli_Sck> Cuma_Client::server_list(){
    return srv_lst_;
}

//파일을 파편화 하여 f_frag_lst_에 보관함
bool Cuma_Client::_file_frag(){
    
    try{
        ifstream r_file;
        unsigned long long f_siz;
        shared_ptr<char> f_chr_to_str_tmp(new char[512]);
        
        //파일 크기 탐색
        r_file.seekg(std::ios::end);
        f_siz = r_file.tellg();
        r_file.seekg(std::ios::beg);
        
        
        //파일 이 eof될때가지 바이너리로 읽기
        while(!r_file.eof()){
            
            //char 512바이트 파일버퍼 초기화
            memset(f_chr_to_str_tmp.get(), 0, 512);
            
            //char로 된 f_tmp에 512바이트를 읽음
            r_file.read(f_chr_to_str_tmp.get(), 512);
            
            //f_buff_에 읽은 512바이트를 읽음
            f_buff_.append(f_chr_to_str_tmp.get(),512);
            
        }
        
        //f_tmp를 delete함
        f_chr_to_str_tmp.reset();
        
        //각 파편화 프레임 크기
        f_fme_siz_ = (f_siz/srv_lst_.size());
        
        
        //등록된 서버 리스트수대로 파일을 나눠서 f_frag_lst 에 저장
        for( int i = 0 ; i < server_list().size(); i++){
            
            Json::Value f_val;
            f_val["F_name"] = f_name_;
            f_val["F_frame_num"] = i;
            f_val["F_binary"] = f_buff_.substr( (i * f_fme_siz_) ,
                                               (i + f_fme_siz_));
            f_val["F_siz"] = f_fme_siz_;
            f_val["MODE"] = WRITE_BINARY;
            
            //i = 프레임 첫번호 ~ i + frag_siz(각 프레임별 버퍼 크기)
            f_frag_lst_.push_back(f_val);
            
            f_val.clear();
        }
        return true;
        
        
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }
}



//파일을 서버로 전송함
bool Cuma_Client::_file_snd(){
    try{
        Json::StyledWriter J_writ;
        
        auto f_it = f_frag_lst_.begin();
        auto f_cli = srv_lst_.begin();
        
        for(int i = 0; i < srv_lst_.size(); i ++){
            
            //각각 파일 프레임 + 서버 리스트를 묶어서 전송함
            std::advance(f_it, i);
            std::advance(f_cli,i);
            
            string f_snd;
            f_snd.append(J_writ.write((*f_it)),
                         J_writ.write((*f_it)).size());
            
            sockaddr_in sock_temp = f_cli->srv_sck_addr();
            connect(f_cli->srv_sck(),
                    (struct sockaddr*)&sock_temp,
                    sizeof(f_cli->srv_sck_addr()));
            
            send(f_cli->srv_sck(), f_snd.c_str(), f_snd.length(), 0);
            
        }
        
        return true;
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }
}

/*//파일 수신 함수
bool Cuma_Client::_file_rcv(){
    try{
        
        
        return true;
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }
    
}*/
