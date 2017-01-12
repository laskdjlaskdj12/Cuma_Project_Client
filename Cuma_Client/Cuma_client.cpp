//
//  Cuma_client.cpp
//  Cuma_Client
//
//  Created by ace on 2016. 12. 30..
//  Copyright © 2016년 ace. All rights reserved.
//

#include "Cuma_client.hpp"


Cuma_Client::Cuma_Client(){
    
    
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
    _CS_LOG(C_F_ACTIVE);
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
bool Cuma_Client::set_file_name(const string path){
    
    ifstream CS_FILE(path,std::ios::binary);
    
    if(CS_FILE.is_open() != true){
        _CS_LOG(C_F_OPEN_FALSE);
        CS_FILE.close();
        return false;
    }
    
    f_name_ = path;
    
    CS_FILE.close();
    
    _CS_LOG(C_F_OPEN);
    return true;
}


string Cuma_Client::file_name(){
    return f_name_;
}









//리스트에 삽입된 서버ip와 port를 생성후 sockaddr_in 구조체 생성
void Cuma_Client::add_server_lst(const std::string s,
                                 int prt){
    
    Cli_Sck s_temp;
    s_temp.set_sock();
    s_temp.set_addr(s.c_str());
    s_temp.set_prt(prt);
    s_temp.set_sck_addr();
    srv_lst_.push_back(s_temp);
    
    _CS_LOG(C_F_ADD_SERV);
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
    if(_file_frag() != true){
        _CS_LOG(C_F_FLE_FRAG,false);
    }
    
    //파일 전송 시작
    if(_file_snd() != true){
        _CS_LOG(C_F_FLE_SND,false);
    }
    _CS_LOG(C_F_FLE_SND_SERVER_CLR);
    
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
        char* f_chr_to_str_tmp = new char[512];
        
        //파일 오픈
        r_file.open(f_name_,std::ios::binary);
        _CS_LOG(C_F_FLE_SND,true);
        
        //파일 크기 탐색
        r_file.seekg(std::ios::end);
        f_siz = r_file.tellg();
        r_file.seekg(std::ios::beg);
        _CS_LOG(C_F_FLE_SIZ,f_siz);
        
        //파일 이 eof될때가지 바이너리로 읽기
        while(!r_file.eof()){
            
            //char 512바이트 파일버퍼 초기화
            memset(f_chr_to_str_tmp, 0, 512);
            
            //char로 된 f_tmp에 512바이트를 읽음
            r_file.read(f_chr_to_str_tmp, 512);
            
            //f_buff_에 읽은 512바이트를 읽음
            f_buff_.append(f_chr_to_str_tmp,512);
        }
        _CS_LOG(C_F_FLE_BNYSIZ,f_buff_.size());
        
        
        //f_tmp를 delete함
        delete[] f_chr_to_str_tmp;
        
        //각 파편화 프레임 크기
        f_fme_siz_ = (f_buff_.size()/srv_lst_.size());
        _CS_LOG(C_F_FLE_EACH_FRM_COUNT,f_fme_siz_);
        
        
        //등록된 서버 리스트수대로 파일을 나눠서 f_frag_lst 에 저장
        for( int i = 0 ; i < srv_lst_.size()+1; i++){
            
            Json::Value f_val;
            f_val["F_name"] = (f_name_+".Cuma_Client"+ std::to_string(i));
            f_val["F_frame_num"] = i;
            //만약 해당 프레임 범위가 파일 바이너리 버퍼 크기를 벗어났을 경우 바이너리 버퍼 크기만큼 분할 사이즈를 설정
            if(i * f_fme_siz_ > f_buff_.size()){
                f_val["F_binary"] = f_buff_.substr( (i * f_fme_siz_) , f_buff_.size() - (f_fme_siz_ * i));
            }
            else{
                f_val["F_binary"] = f_buff_.substr( (i * f_fme_siz_) , f_fme_siz_);
            }
            f_val["F_siz"] = (Json::UInt64)(f_val["F_binary"].asString().length());
            f_val["MODE"] = WRITE_BINARY;
            
            
            f_frag_lst_.push_back(f_val);
            
            f_val.clear();
        }
        _CS_LOG(C_F_FLE_SPLT_FRAME,true);
        
        
        //파일 프레임 으로 나뉘어진 json 보여주기
        for(list<Json::Value>::iterator it = f_frag_lst_.begin(); it != f_frag_lst_.end(); it++){
            
            std::cout<<"============ Json_Value ============= "<<std::endl;
            std::cout<<"F_name : "<<(*it)["F_name"].asString()<<std::endl;
            std::cout<<"F_frame_num : "<<(*it)["F_frame_num"].asUInt64()<<std::endl;
            std::cout<<"F_siz : "<<(*it)["F_siz"].asUInt64()<<std::endl;
            std::cout<<"MODE : "<<(*it)["MODE"].asInt()<<std::endl;             //mode 0 = READ_BINARY, mode 1 = WRITE_BINARY
            
        }
        _CS_LOG(C_F_FLE_SHOW_FRAAME);
        
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
        
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(const string& s){
        _CS_LOG(s,false);
        return false;
        
    }
}















//파일을 서버로 전송함
bool Cuma_Client::_file_snd(){
    try{
        Json::StyledWriter J_writ;
        Json::Value Con_Chk;                                               //json형식으로 응답을 받음
        Json::Value J_Frag_save;                                           //파편화 캐시 저장
        Json::Value J_Frag_addr;
        
        
        for(int i = 0; i<srv_lst_.size(); i++){
            auto C_F_srv_tmp = srv_lst_.begin();
            std::advance(C_F_srv_tmp, i);
            
            std::cout<<"[Info] : ["<<i<<"] : "<<inet_ntoa(C_F_srv_tmp->srv_sck_addr().sin_addr)<<":"<<htons((C_F_srv_tmp->srv_sck_addr().sin_port))<<std::endl;
        }
        
        
        for(int i = 0; i < f_frag_lst_.size(); i ++){
            auto C_F_frag = f_frag_lst_.begin();
            auto C_F_srv = srv_lst_.begin();
            
        
            //각각 파일 프레임 + 서버 리스트를 묶어서 전송함
            std::advance(C_F_frag, i);                                      //파일 파편화 인덱스
            std::advance(C_F_srv,  (i % (srv_lst_.size())));                //서버 소켓 인덱스
            
            
            
            //json프레임 저장
            J_Frag_addr.clear();                                            //Json 버퍼를 clear하고
            J_Frag_addr["F_addr"].append(inet_ntoa(C_F_srv->srv_sck_addr().sin_addr));
            J_Frag_save[(*C_F_frag)["F_name"].asString()].append(J_Frag_addr);
            
            string f_snd;                                                   //JSON 전송 버퍼
            f_snd.append(J_writ.write((*C_F_frag)),
                         J_writ.write((*C_F_frag)).size());
            
            sockaddr_in sock_temp = C_F_srv->srv_sck_addr();
            
            std::cout<<"[Info] : "<<inet_ntoa(sock_temp.sin_addr)<<":"<<htons(sock_temp.sin_port)<<" connecting..."<<std::endl;
            
            if(connect(C_F_srv->srv_sck(),
                       (struct sockaddr*)&sock_temp,
                       sizeof(C_F_srv->srv_sck_addr())) < 0){
                throw errno;
            };
            _CS_LOG(_CS_CON_STAT,true);
            
            
            //서버에서 접속 응답을 받음
            _CS_RCV(C_F_srv->srv_sck(), Con_Chk);
            _CS_LOG(_CS_CON_SRV_RCV,true);
            
            //만약 서버에서 에러를 응답했을경우
            if(Con_Chk["RCV_ERR"].isObject()){  throw string(_CS_CON_SRV_STAT); }
            
            //만약 서버에서 에러를 응답하지 않았을 경우
            if(_CS_SND(C_F_srv->srv_sck(), *(&*C_F_frag)) != true){ throw string(_CS_SND_FAIL); };
            if(_CS_RCV(C_F_srv->srv_sck(), *(&*C_F_frag)) != true){ throw string(_CS_RCV_FAIL); };
            
            //만약 서버측 응답이 Error일 경우
            if((*(&*C_F_frag)).isMember("Error") != false){
                std::cout<<"[Error] : Server _ "<<(*(&*C_F_frag))["Raseon"].asCString()<<std::endl;
                throw string(_CS_SRV_RCV);
            }
            //서버가 성공적으로 저장됨을 알림
            std::cout<<"[Info] : Recv_Serv : ["<<inet_ntoa(C_F_srv->srv_sck_addr().sin_addr)<<": "<<htons(C_F_srv->srv_sck_addr().sin_port)<<"] :";
            std::cout<<(*(&*C_F_frag))["F_name"].asString();
            std::cout<<" : "<<(*(&*C_F_frag))["F_siz"].asUInt64()<<" byte "<<std::endl;
            
            
            std::cout<<"[Info] : SRV_SUCCES_SAVE_frame"<<std::endl;
        }
        
        ofstream w_binary(J_Frag_save["F_name"].asString());
        string w_temp = J_writ.write(J_Frag_save);
        w_binary.write(w_temp.c_str(), w_temp.size());
        
        w_temp.clear();
        Con_Chk.clear();
        J_Frag_save.clear();
        J_Frag_addr.clear();
        
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }catch(const string s){
        _CS_LOG(s,false);
        return false;
    }
}











void Cuma_Client::_CS_LOG(const string& s){
    std::cout<<"[Info] : "<<s<<std::endl;
}

void Cuma_Client::_CS_LOG(const string& s , unsigned long long siz){
    std::cout<<"[Info] : "<<s<<" : "<<siz<<std::endl;
}

/*void Cuma_Client::_CS_LOG(const string& s , bool a){
 if(a != true){  std::cout<<"[Info] : "<<s<<" : "<<"fail"<<std::endl;}
 else{ std::cout<<"[Info] : "<<s<<" : "<<"clear"<<std::endl;}
 }*/












bool Cuma_Client::_CS_SND(const int s, Json::Value& J){
    try{
        
        Json::StyledWriter _CS_WRI;
        string _CS_TMP = _CS_WRI.write(J);
        unsigned long long _CS_TMP_LEN = _CS_TMP.length();
        // unsigned long long _CS_SND_BYTE_TEMP;
        
        send(s, &_CS_TMP_LEN, sizeof(unsigned long long), 0);
        
        unsigned long long _CS_SND_BYTE = send(s, _CS_TMP.c_str(),(_CS_TMP.length()),0);
        
        //inidicate로 전송을 나타내기
        /* while(_CS_SND_BYTE_TEMP != _CS_TMP_LEN){
         _CS_INDI(_CS_TMP_LEN, _CS_SND_BYTE);
         _CS_SND_BYTE_TEMP += _CS_SND_BYTE;
         }*/
        _CS_LOG("Send Byte of ",_CS_SND_BYTE);
        _CS_LOG(_CS_SND_CLR, true);
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[ERR] : "<<e.what()<<std::endl;
        _CS_LOG(_CS_SND_FAIL, false);
        return false;
    }
    
}














bool Cuma_Client::_CS_RCV(const int s, Json::Value& J){
    
    try{
        
        Json::Reader _CS_RED;
        unsigned long long _CS_TMP_SIZ;
        string _CS_TMP_BINARY;
        
        recv(s, &_CS_TMP_SIZ, sizeof(unsigned long long),0);
        
        shared_ptr<char> _CS_TMP_CHAR(new char[_CS_TMP_SIZ]);
        recv(s, (&*_CS_TMP_CHAR), _CS_TMP_SIZ,0);
        
        _CS_TMP_BINARY.append((&*_CS_TMP_CHAR),_CS_TMP_SIZ);
        _CS_RED.parse(_CS_TMP_BINARY, J);
        
        _CS_LOG(_CS_RCV_CLR, true);
        
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[INFO] : "<<e.what()<<std::endl;
        _CS_LOG(_CS_RCV_FAIL, false);
        return false;
    }
}


void Cuma_Client::_CS_INDI(unsigned long long f_siz, unsigned long long snd_siz){
    
    std::cout<<"[";
    for(int i  = 0; i<(int)((snd_siz/f_siz)* 50); i++){
        std::cout<<"=";
    }
    std::cout<<"] "<<(int)((snd_siz/f_siz)* 50)<<"% \r";
    
}
