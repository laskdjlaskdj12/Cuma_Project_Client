//
//  main.cpp
//  Cuma_Client
//
//  Created by ace on 2016. 12. 29..
//  Copyright © 2016년 ace. All rights reserved.
//

#include <iostream>
#include "Cuma_client.hpp"




int main(int argc, const char * argv[]) {
    shared_ptr<Cuma_Client> c(new Cuma_Client);
    
    /*while(c->is_active()){
        int panel = 0;
        while(panel != 0){
            std::cout<<"[0] : insert_Server" <<std::endl;
            std::cout<<"[1] : insert_File "<<std::endl;
            std::cout<<"[2] : show_Server"<<std::endl;
            std::cout<<"[3] : start"<<std::endl;
            std::cout<<"[4] : exit"<<std::endl;
            panel = getchar();
            
            if(panel < 0 || panel > 2){
                std::cout<<"[Info] : Wrong panel"<<std::endl;
                continue;
            }
        }
        
        switch (panel) {
            case 0:{
                string ip;
                int prt;
                
                std::cout<<"Server_ip: <<"; std::cin>>ip;
                std::cout<<"Server_port: "; std::cin>>prt;
                
                c->add_server_lst(ip, prt);
                break;
            }
            case 1:{
                string f_p;
                
                std::cout<<"File_Path: <<"; std::cin>>f_p;
                
                c->set_file_name(f_p);
                
                break;
            }
            case 2:{
                c->show_server_list();
                break;
            }
            case 3:{
                c->start();
                c->send_server_list();
                break;
            }
            case 4:{
                c->stop();
                break;
            }
            default:{
                std::cout<<"[Error] : Wrong_index"<<std::endl;
                c->stop();
                break;
            }
        }
    }
    
    //exit함
    exit(1);*/
    
    
    c->start();
    
    c->add_server_lst("127.0.0.1", 33390);
    c->add_server_lst("127.0.0.1", 33391);
    c->add_server_lst("127.0.0.1", 33392);
    c->add_server_lst("127.0.0.1", 33393);
    c->add_server_lst("127.0.0.1", 33394);
    c->add_server_lst("127.0.0.1", 33395);
    
    c->show_server_list();
    
    c->send_server_list();
    
}
