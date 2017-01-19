

#Cuma_Client_Remake

##Cuma Project

Cluster unit memory architecture 의 약자인 Cuma는 다중 서버를 기반으로 파일 분할을 통해 온라인 파일 스토어를 하는 방식입니다. Cuma 의 기초적인 작동 방식은 다음와 같습니다.

파일 전송프로토콜 로서 파일을 클라이언트에서 지정된 서버의 갯수만큼 프레임을 분활시켜서 전송을 하는 방식으로 송신시 지정된 서버에 접속을 하는것 이외 중계노드에 접속 단계를 거치지 않습니다.

#Cuma_Client

본 소스는 Cuma 프로토콜의  코어 소스로써 기본적인 코어 기능만을 포함하고 있습니다.


#Cuma Project

Cluster unit memory architecture (A.K.A Cuma) is way to store file based on Multi Base Server. As File Send protocol, when file is requested send by Client, file splits amount of Selected Cuma_Server list count and send it direct to Server. Althought there is no contect any repeter server.

##Cuma_Client
About this Source is Client prototype Source that include Basic Core functional.

Copyright (c) 2016  laskdjlaskdj12 allright reserved.
