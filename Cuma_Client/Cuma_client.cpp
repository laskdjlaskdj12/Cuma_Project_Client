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
    f_buff_temp_.clear();
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

void Cuma_Client::set_mode(int n){
    mode_ = n;
}










//파일 선택
bool Cuma_Client::set_file_name(const string path){
    
    //만약 mode_가 WRITE_BINARY일때
    if(mode_ == WRITE_BINARY){
    ifstream CS_FILE(path,std::ios::binary);
    
    if(CS_FILE.is_open() != true){
        _CS_LOG(C_F_OPEN_FALSE);
        CS_FILE.close();
        return false;
    }
    
    f_name_ = path;
    
    CS_FILE.close();
    
    _CS_LOG(C_F_OPEN);
        
    }else{
        f_name_ = path;
    }
    
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
bool Cuma_Client::send_to_server_list(){
    
    //파일 파편화 시작
    if(_file_frag() != true){
        _CS_LOG(C_F_FLE_FRAG,false);
        return false;
    }
    
    //파일 전송 시작
    if(_file_snd() != true){
        _CS_LOG(C_F_FLE_SND,false);
        return false;
    }
    _CS_LOG(C_F_FLE_SND_SERVER_CLR);
    return true;
}


//recv에서는 send와 반대로 파일 프레임에 대해 수신을 먼저 받고 파편화로 넘김

bool Cuma_Client::recv_from_server_list(){
    
    //해당 파일을 전송한 .Cuma인덱스 파일을 읽음
    if(_file_init_assay() != true){
        _CS_LOG(C_F_FLE_RCV,false);
        return false;
    }
    
    //파일 을 수신후 저장
    if(_file_rcv() != true){
        _CS_LOG(C_F_FLE_FRAG_READ,false);
        return false;
    }
    
    // 파일 프레임을 재조합하여 하나의 파일로 재구성
    if(_file_combine() != true){
        _CS_LOG(C_F_FLE_INIT,false);
    }
    _CS_LOG(C_F_FILE_RECV_SERVER_CLR);
    return true;
}









list<Cli_Sck> Cuma_Client::server_list(){
    return srv_lst_;
}









//파일을 파편화 하여 f_frag_lst_에 보관함
bool Cuma_Client::_file_frag(){
    
    try{
        
        ifstream r_file;
        unsigned long long f_siz = 0;
        char* f_chr_to_str_tmp = new char[512];
        int frag_bit = 0;
        
        
        //파일 오픈
        r_file.open(f_name_,std::ios::binary);
        _CS_LOG(C_F_FLE_SND,true);
        
        //파일 크기 탐색
        r_file.seekg(std::ios::beg,std::ios::end);
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
            f_buff_temp_.append(f_chr_to_str_tmp,512);
        }
        _CS_LOG(C_F_FLE_BNYSIZ,f_buff_temp_.size());
        
        
        //f_tmp를 delete함
        delete[] f_chr_to_str_tmp;
        
        //각 파편화 프레임 크기
        f_fme_siz_ = (f_buff_temp_.size()/srv_lst_.size());
        _CS_LOG(C_F_FLE_EACH_FRM_COUNT,f_fme_siz_);
        
        //프래그 비트 설정 (만약 프레임의 나머지가 있을 경우
        if((f_buff_temp_.size() % f_fme_siz_) > 0){ frag_bit = 1; }
        
        //등록된 서버 리스트수대로 프레임 파일을 생성후 f_frag_lst 에 저장
        for( int i = 0 ; i < srv_lst_.size() + frag_bit ; i++){
            
            //프래그먼트에 파일 이름을 설정함
            Cuma_Frame F;
            F.set_f_name(f_name_);
            F.set_fr_name( f_name_ + ".Cuma_Client" + std::to_string( i ) );
            F.set_fr_num( i );
            
            //만약 해당 프레임 범위가 파일 바이너리 버퍼 크기를 벗어났을 경우 바이너리 버퍼 크기만큼 분할 사이즈를 설정
            if( i * f_fme_siz_ > f_buff_temp_.size() ){
                F.set_fr_binary( f_buff_temp_.substr( (i * f_fme_siz_) ,
                                                f_buff_temp_.size() - (f_fme_siz_ * i)));
            }
            
            else{
                F.set_fr_binary (f_buff_temp_.substr( (i * f_fme_siz_) ,
                                                  f_fme_siz_) );
            }
            
            //프레임 크기를 입력
            F.set_fr_siz((Json::UInt64)(F.fr_binary().length()));
            F.set_fr_MOD( WRITE_BINARY );
            
            
            //프레임 리스트에 push_back
            f_frag_lst_.push_back(F);
            
            
        }
        _CS_LOG(C_F_FLE_SPLT_FRAME,true);
        
        
        //파일 프레임 으로 나뉘어진 json 보여주기
        for(list<Cuma_Frame>::iterator it = f_frag_lst_.begin(); it != f_frag_lst_.end(); it++){
            
            std::cout<<"============ Json_Value ============= "<<std::endl;
            std::cout<<"F_name : "<<        it->f_name() <<std::endl;
            std::cout<<"F_frame_num : "<<   it->fr_name() <<std::endl;
            std::cout<<"F_siz : "<<         it->fr_siz() <<std::endl;
            std::cout<<"MODE : "<<          it -> fr_MOD() <<std::endl;
            std::cout<<"============ End_JSON_Value =============="<<std::endl;
            //mode 0 = READ_BINARY, mode 1 = WRITE_BINARY
            
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
        Json::Value CS_CON_STAT;                                               //json형식으로 응답을 받음
        Json::Value J_Frag_addr;                                               //json의 .Cuma 저장형식으로 저장함
        Json::Value CS_RES;                                                    //json 리턴값 결과
        
        for(int i = 0; i<srv_lst_.size(); i++){
            auto C_F_srv_tmp = srv_lst_.begin();
            
            std::advance(C_F_srv_tmp, i);
            
            std::cout<<"[Info] : ["<<i<<"] : "<<inet_ntoa(C_F_srv_tmp->srv_sck_addr().sin_addr)<<":"<<htons((C_F_srv_tmp->srv_sck_addr().sin_port))<<std::endl;
        }
        
        //파일 파편화 갯수대로 서버 큐가 원형 연결리스트로 연결되어서 전송을 하게됨
        for(int i = 0; i < f_frag_lst_.size(); i ++){
            
            auto C_Frame = f_frag_lst_.begin();            //파일 프레임 iterator
            auto C_srv = srv_lst_.begin();                //서버 리스트 iterator
            
            
            //각각 파일 프레임 + 서버 리스트를 묶어서 전송함
            std::advance(C_Frame, i);                      //파일 파편화 인덱스
            std::advance(C_srv,  (i % srv_lst_.size()));  //서버 소켓 인덱스 (서버 를 힙으로 해서 로테이션하여 프레임을 서버에 전송)
            
            
            //파일 이름에 프레임 수대로 하위 오브젝트에 J_frag_addr를 import
            C_Frame->set_fr_addr( (string)inet_ntoa( C_srv -> srv_sck_addr().sin_addr ) ) ;
            C_Frame->set_fr_prt( C_srv -> srv_prt());
            f_r_send_lst["F_name"] = f_name_;
            f_r_send_lst[f_name_].append( C_Frame -> to_json());
            f_r_send_lst["F_frame_all_num"] = (Json::UInt)f_frag_lst_.size();
            _CS_LOG(_CS_FRAME_LOG,true);
            J_Frag_addr.clear();
            
            
            //////////////////////////////////////////////Cuma_Server 접속 영역
            
            //JSON 전송 버퍼
            string f_snd;
            f_snd.append(J_writ.write(C_Frame -> to_json()),
                         J_writ.write(C_Frame -> to_json()).size());
            
            sockaddr_in sock_temp = C_srv -> srv_sck_addr();
            
            std::cout<<"[Info] : "<<inet_ntoa(sock_temp.sin_addr)<<":"<<htons(sock_temp.sin_port)<<" connecting..."<<std::endl;
            
            if(connect(C_srv -> srv_sck(),
                       (struct sockaddr*)&sock_temp,
                       sizeof(C_srv -> srv_sck_addr())) < 0){
                throw string(_CS_CON_FAIL);
            };
            _CS_LOG(_CS_CON_STAT,true);
            
            
            //서버에서 접속 응답을 받음
            if(_CS_RCV(C_srv -> srv_sck(), CS_CON_STAT) != true){ throw string(_CS_SND_FAIL); }
            _CS_LOG(_CS_CON_SRV_RCV,true);
            
            //////////////////////////////////////////////
            
            
            //만약 서버에서 에러를 응답했을경우
            if(CS_CON_STAT["RCV_ERR"].isObject()){  throw string(_CS_CON_SRV_STAT); }
            
            Json::Value CS_SND = C_Frame -> to_json();
            
            //만약 서버에서 에러를 응답하지 않았을 경우
            if(_CS_SND(C_srv -> srv_sck(), CS_SND) != true){ throw string(_CS_SND_FAIL); }
            if(_CS_RCV(C_srv -> srv_sck(), CS_RES) != true){ throw string(_CS_RCV_FAIL); }
            
            
            //만약 서버측 응답이 Error일 경우
            if(CS_RES.isMember("Error") != false){
                std::cout<<"[Error] : Server _ "<<CS_RES["Raseon"].asCString()<<std::endl;
                throw string(_CS_SRV_RCV);
            }
            
            
            //서버가 성공적으로 저장됨을 알림
            std::cout<<"[Info] : Recv_Serv : ["<<inet_ntoa(C_srv -> srv_sck_addr().sin_addr)<<": "<<htons(C_srv -> srv_sck_addr().sin_port)<<"] :";
            std::cout<<CS_RES["F_name"].asString();
            std::cout<<" : "<<CS_RES["F_siz"].asUInt64()<<" byte "<<std::endl;
            
            
            std::cout<<"[Info] : SRV_SUCCES_SAVE_frame"<<std::endl;
            
            
            //해당 소켓 셧다운 후 소켓 디스크립터 재생성
            shutdown(C_srv -> srv_sck(), SHUT_RDWR);
            close(C_srv -> srv_sck());
            C_srv -> set_sock();
            
            
        }
        
        
        
        //해당 오브젝트를 인덱스 형태로 저장
        ofstream w_binary;
        std::stringstream f_name;
        f_name<<f_r_send_lst["F_name"].asString()<<".Cuma";
        
       /* std::cout<<"=========== Debug Json ============="<<std::endl;
        std::cout<<f_r_send_lst<<std::endl;
        std::cout<<"=========== End Json ============="<<std::endl;*/
        
        
        w_binary.open(f_name.str(), std::ios::beg);
        string w_temp = J_writ.write(f_r_send_lst);
        w_binary.write(J_writ.write(f_r_send_lst).c_str(), w_temp.size());
        
        
        w_binary.close();
        w_temp.clear();
        CS_CON_STAT.clear();
        f_r_send_lst.clear();
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





//.Cuma인덱스 파일을 오픈해서 파편화 인덱스 서버를 리스트에 push 역활
bool Cuma_Client::_file_init_assay(){
    try{
        
        //ifstream으로 해당 파일을 open함
        std::stringstream temp;
        temp<<f_name_<<".Cuma";
        
        ifstream idx_read(temp.str().c_str());
        
        
        if(idx_read.is_open() != true ){
            _CS_LOG("Cuma_File not open",false);
            return false;
        }
        
        
        idx_read.seekg(std::ios::beg, std::ios::end);
        unsigned long long f_siz = idx_read.tellg();
        idx_read.seekg(std::ios::beg);
        
        
        char f_buff[f_siz];
        
        //f_buff로 인덱스를 읽기
        idx_read.read(f_buff, f_siz);
        
        //Json을 parse함
        Json::Reader f_reader;
        f_reader.parse(f_buff, frame_save_lst);
        
        //읽어진 .Cuma파일을 parse해서 프레임 추출후 f_frag_lst_에 push함
        for(int i = 0; i < frame_save_lst["F_frame_all_num"].asInt(); i++){
            Cuma_Frame Frame;
            Json::Value frag;
            
            frag["F_name"] =        frame_save_lst["F_name"].asString();
            frag["F_addr"] =        frame_save_lst[f_name_][i]["F_addr"].asString();
            frag["F_port"] =        frame_save_lst[f_name_][i]["F_port"].asInt();
            frag["F_frame_num"] =   frame_save_lst[f_name_][i]["F_frame_num"].asInt();
            frag["F_siz"] =         frame_save_lst[f_name_][i]["F_siz"].asUInt64();
            frag["MODE"] =         READ_BINARY;
            
            Frame.set_fr_json(frag);
            Frame.json_to_mem();                //json을 멤버변수로 변환하기
            
            f_frag_lst_.push_back(Frame);
        }
        
        //ifstream close
        idx_read.close();
        
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
        
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
        
    }catch(const string& e){
        _CS_LOG(e,false);
        return false;
        
    }
}

//_file_rcv은 프레임을 중심으로 저장된 서버리스트를 할당하고 있음 고로 중복된 서버 리스트가 있으므로 connect를 중복으로 작성함
bool Cuma_Client::_file_rcv(){
    try{
        
        //현재 남아있는 서버 인덱스 들은 모두 clear
        srv_lst_.clear();
        
        //프레임 리스트의 프레임에 적혀있는 서버 ip prt 추가
        for(list<Cuma_Frame>::iterator ite = f_frag_lst_.begin(); ite != f_frag_lst_.end(); ite++){
            
            //해당 프레임을 받을 서버를 추가
            this->add_server_lst(ite->fr_addr(), ite->fr_prt());
        }
        
        //리스트 프레임의 json을 출력
        for(list<Cuma_Frame>::iterator ite = f_frag_lst_.begin(); ite != f_frag_lst_.end(); ite++){
            std::cout<<ite->to_json()<<std::endl;
        }
        
        for(list<Cli_Sck>::iterator ite = srv_lst_.begin(); ite != srv_lst_.end(); ite++){
            
            std::cout<<"[Info] : Connecting "<<inet_ntoa((*ite).srv_sck_addr().sin_addr)<<":"<<(*ite).srv_prt()<<std::endl;
        }
        
        
        //해당 프레임을 받을 서버에 전송 및 대기
        Json::Value CS_CON_STAT;
        Json::Value CS_FARG;
        Json::Value CS_RCV;
        
        
        //서버 프레임에 대한 검증은 서버리스트를 입력할때 처리하기로 함
        //불러온 파일 파편화 리스트의 갯수 만큼
        
        
        for(int i = 0 ; i < srv_lst_.size(); i++){
            
            auto ite_sock = srv_lst_.begin();
            auto ite_frame = f_frag_lst_.begin();
            
            std::advance(ite_sock, i);
            std::advance(ite_frame, i);
            
            
            //소켓와 sockaddr를 바인딩
            sockaddr_in temp_sck_addr_ = ite_sock->srv_sck_addr();
            
            //서버 접속
            if(connect(ite_sock->srv_sck(), (sockaddr*)&temp_sck_addr_, sizeof(temp_sck_addr_))){
                throw string(_CS_CON_FAIL);
            }
            _CS_LOG(_CS_CON_SRV_STAT,true);
            
            
            //서버접속후 rcv받은것이 있는지 확인
            if(!_CS_RCV(ite_sock->srv_sck(), CS_CON_STAT)){throw string(_CS_CON_FAIL);}
            _CS_LOG(_CS_CON_SRV_RCV,true);
            
            
            //만약 서버 접속수 RCV_ERR가 리턴이 됬을시
            if(CS_CON_STAT["Error"].isObject()){ throw string(_CS_CON_FAIL);}
            
            
            //수신을 받을 프레임들을 받을 리스트를 작성
            CS_FARG = ite_frame->fr_json();
            
            
            
            //리스트 전송와 수신
            if(_CS_SND(ite_sock->srv_sck(), CS_FARG) != true){ throw string(_CS_SND_FAIL); }
            if(_CS_RCV(ite_sock->srv_sck(), CS_RCV) != true){ throw string(_CS_RCV_FAIL); }
            
            
            //만약 에러를 리턴하였을 경우 reason을 출력
            if(CS_RCV.isMember("Error") == true){
                std::cout<<"[Error] : Server _ "<<CS_RCV["Raseon"].asCString()<<std::endl;
                throw string(_CS_SRV_RCV);
            }
            
            //std::cout<<"[Debug] : "<<CS_RCV<<std::endl;
            
            //프레임에 json을 입력
            Cuma_Frame Res_frame;
            Res_frame.set_fr_json(CS_RCV);
            Res_frame.json_to_mem();
            
            //해당 수신된 프레임을 recV_frag_lst_에 저장함
            rcv_frag_lst_.push_back(Res_frame);
            
            
            //클라이언트가 저장 성공 로그
            std::cout<<"[Info] : Recv_Serv : ["<<inet_ntoa(ite_sock->srv_sck_addr().sin_addr)<<": "<<htons(ite_sock->srv_sck_addr().sin_port)<<"] :";
            std::cout<< CS_RCV["F_name"].asString();
            std::cout<<" : "<<CS_RCV["F_siz"].asUInt64()<<" byte "<<std::endl;
            std::cout<<"[Info] : CLI_SUCCES_SAVE_frame"<<std::endl;
            
            
            //클라이언트 소켓 close한후 재활용을 위해 socket 디스크립터 재생성
            shutdown(ite_sock->srv_sck(), SHUT_RDWR);
            close(ite_sock->srv_sck());
            ite_sock->set_sock();
            
        }
        return true;
        
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
        
    }catch(const string& e){
        std::cout<<"[Error] : "<<e<<std::endl;
        return false;
        
    }catch(Json::Exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }
}


//수신한 파일 파편화 조각들을 통해 파일 재조합
bool Cuma_Client::_file_combine(){
    try{
        string w_bin;
        
        for(int i = 0; i < rcv_frag_lst_.size(); i ++){
            
            //프레임 순서대로 sort
            for(std::list<Cuma_Frame>::iterator ite = rcv_frag_lst_.begin(); ite != rcv_frag_lst_.end(); ite++){
                
                if(ite -> fr_num() == i){
                    
                    std::cout<<"[Debug] : fr_binary size = "<<(ite->fr_binary()).size()<<", fr_size = "<<(ite)->fr_siz()<<std::endl;
                    
                    
                    w_bin.append((ite->fr_binary()).c_str() , ite->fr_siz());
                    
                    break;
                }
            }
        }
        
        ofstream f_writ(f_name_,std::ios::binary | std::ios::beg);
        
        f_writ.write(w_bin.c_str(), w_bin.size());
        
        f_writ.close();
        
        
        
        return true;
    }catch(std::exception& e){
        std::cout<<"[Error] : "<<e.what()<<std::endl;
        return false;
    }
}







void Cuma_Client::_CS_LOG(const string& s){
    std::cout<<"[Info] : "<<s<<std::endl;
}

void Cuma_Client::_CS_LOG(const string& s , unsigned long long siz){
    std::cout<<"[Info] : "<<s<<" : "<<siz<<std::endl;
}








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
        
        //JSON::READER
        Json::Reader _CS_RED;
        unsigned long _CS_TMP_SIZ;
        string _CS_TMP_BINARY;
        char* _CS_TMP_CHAR;
        
        unsigned long rcv_temp = 0;
        unsigned long rcv_temp1 = 0;
        
        //사이즈를 수신받음
        recv(s, &_CS_TMP_SIZ, sizeof(unsigned long),0);
        
        //바이트 크기를 할당함
        _CS_TMP_CHAR = (new char[_CS_TMP_SIZ]);
        memset(_CS_TMP_CHAR, 0, _CS_TMP_SIZ);
        
        //수신이 완료가 될때까지 string으로 수신
        while( rcv_temp < _CS_TMP_SIZ ){
            
            rcv_temp1 = recv(s, _CS_TMP_CHAR, _CS_TMP_SIZ,0);
            rcv_temp += rcv_temp1;
            
            _CS_TMP_BINARY.append( _CS_TMP_CHAR , rcv_temp1);
            
        }
        
        //json으로 변환
        _CS_RED.parse(_CS_TMP_BINARY, J);
        
        _CS_LOG(_CS_RCV_CLR, true);
        
        delete[] _CS_TMP_CHAR;
        
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
