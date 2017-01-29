

#Cuma_Client_Remake

##Cuma Project

Cluster unit memory architecture 의 약자인 Cuma는 다중 서버를 기반으로 파일 분할을 통해 온라인 파일 스토어를 하는 방식입니다. Cuma 의 기초적인 작동 방식은 다음와 같습니다.

파일 전송프로토콜 로서 파일을 클라이언트에서 지정된 서버의 갯수만큼 프레임을 분활시켜서 전송을 하는 방식으로 송신시 지정된 서버에 접속을 하는것 이외 중계노드에 접속 단계를 거치지 않습니다.

##Cuma_Client

본 소스는 Cuma 프로토콜의  코어 소스로써 기본적인 코어 기능만을 포함하고 있습니다.

##설치 순서 
1.  JsonCpp 패키지를 다운받습니다.
      ex) wget https://github.com/open-source-parsers/jsoncpp
      
2.    JsonCpp 패키지를 빌드하여 헤더파일와 소스파일을 생성합니다.
        ex) python amalgamate.py
        
3.    생성된 Jsoncpp/dist 디렉토리를 {Cuma_Client위치}/에 옮김니다.
        ex) mv {Jsoncpp_location}/dist {Project_location} 
        
4.    Cmake를 이용하여 소스코드를 빌드합니다. command: --build . [--config ] [--target ] [-- -i]   
         ex) cmake --build . {Install_Dir}                ==================> 반드시 --build 에 점이 들어가 있을것
        
5.    (4)번에서 빌드된 디렉토리 에서 build 명령어로 실행파일을 생성합니다.   ex) build

6.    끝 


#Cuma Project

Cluster unit memory architecture (A.K.A Cuma) is way to store file based on Multi Base Server. As File Send protocol, when file is requested send by Client, file splits amount of Selected Cuma_Server list count and send it direct to Server. Althought there is no contect any repeter server.

##Cuma_Client

About this Source is Client prototype Source that include Basic Core functional.

##Install Guide


1. download Jsoncpp package from "https://github.com/open-source-parsers/jsoncpp"
   ex) wget "https://github.com/open-source-parsers/jsoncpp"

2. repackage the Jsoncpp for generate header and source file
    ex) python amalgamate.py
    
3. move the "Jsoncpp/dist" directory to "{Project_Loc}/"
    ex) mv {Jsoncpp_location}/dist {Project_location}
    
4. Build the Source Code Using Cmake. command: --build . [--config ] [--target ] [-- -i]   
      ex) cmake --build . {Install_Dir}      ==================> must have dot ahead in "--build"
        
5.  Generate execute file using "build" command in after build in (8) directory.   
      ex) build

6.    done



Copyright (c) 2016  laskdjlaskdj12 allright reserved.
