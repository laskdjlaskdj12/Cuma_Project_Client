//
//  C_Log.hpp
//  Cuma_Client
//
//  Created by ace on 2017. 1. 3..
//  Copyright © 2017년 ace. All rights reserved.
//

#ifndef C_Log_hpp
#define C_Log_hpp

#define Eng


#ifdef Eng
#define C_F_ACTIVE      "Cuma_Clienet is active"
#define C_F_OPEN        "Cuma_Client file is open"
#define C_F_OPEN_FALSE  "Cuma_Client file is not open"
#define C_F_ADD_SERV    "Cuma_Client has add"


#define C_F_FLE_FRAG        "Cuma_Client check _file_frag"
#define C_F_FLE_SND         "Cuma_Client _file_snd"
#define C_F_FLE_SIZ         "Cuma_Client _file_siz"
#define C_F_FLE_BNY         "Cuma_Client _file_buff"
#define C_F_FLE_BNYSIZ      "Cuma_Client _file_binary_SIZ"
#define C_F_FLE_EACH_FRM_COUNT     "Cuma_Client _file_EACH_Frame_count"
#define C_F_FLE_SPLT_FRAME  "Cuma_Client _file_splite_Frame"
#define C_F_FLE_SHOW_FRAAME  "Cuma_Client _Frame_show_Complete"
#define C_F_FLE_SND_SERVER_CLR "Cuma_Client _Send_Server_List_Clear"

#define C_F_FLE_INIT      "Cuma_Client_File_assembly"
#define C_F_FLE_FRAG_READ "Cuma_Client_RECV_FRAG"
#define C_F_FLE_RCV         "Cuma_Client_Recv_file"
#define C_F_FILE_RECV_SERVER_CLR "Cuma_Client_RECV_SERVER_CLR"

#define _CS_CON_STAT    "Cuma_Client _CS_CONN connection"
#define _CS_CON_FAIL    "Cuma_Client connect fail"
#define _CS_SND_CLR     "Cuma_Client _CS_SND_CLR"
#define _CS_SND_FAIL    "Cuma_Client _CS_SND_FAIL"
#define _CS_RCV_CLR     "Cuma_Client _CS_SND_CLR"
#define _CS_RCV_FAIL    "Cuma_Client _CS_SND_FAIL"

#define _CS_CON_SRV_STAT "Cuma_Server _CA_SRV_FULL QUEUE"
#define _CS_CON_SRV_RCV  "Cuma_Server _RCV_CONNECT"


#define _CS_SRV_RCV  "Cuma_Server _RCV_CONNECT_ERROR"

#define _CS_FRAME_LOG "Cuma_Client _success_frame_log_file"


#elif KOR

#define C_F_ACTIVE      "Cuma_Clienet 가 활성화 되었습니다."
#define C_F_OPEN        "Cuma_Client 파일 오픈"
#define C_F_OPEN_FALSE  "Cuma_Client 파일이 열리지 않았습니다."
#define C_F_ADD_SERV    "Cuma_Client 추가"


#define C_F_FLE_FRAG        "Cuma_Client 파일 파편화 상태"
#define C_F_FLE_SND         "Cuma_Client 파일 전송"
#define C_F_FLE_SIZ         "Cuma_Client 파일 크기"
#define C_F_FLE_BNY         "Cuma_Client 파일 바이너리 버퍼"
#define C_F_FLE_BNYSIZ      "Cuma_Client 파일 바이너리 크기"
#define C_F_FLE_EACH_FRM_COUNT     "Cuma_Client 파일 파편화 개수"
#define C_F_FLE_SPLT_FRAME  "Cuma_Client 파일 파편화 프레임"
#define C_F_FLE_SHOW_FRAAME  "Cuma_Client 프레임 오픈 완료"
#define C_F_FLE_SND_SERVER_CLR "Cuma_Client 등록 서버 리스트"
#define C_F_FLE_INIT


#define _CS_CON_STAT    "Cuma_Client 서버 연결 connection"
#define _CS_CON_FAIL    "Cuma_Client 접속 실패"
#define _CS_SND_CLR     "Cuma_Client 서버 전송 완료"
#define _CS_SND_FAIL    "Cuma_Client 서버 전송 실패"
#define _CS_RCV_CLR     "Cuma_Client 서버에서 전송 완료"
#define _CS_RCV_FAIL    "Cuma_Client 서버에서 전송 실패"

#define _CS_CON_SRV_STAT "Cuma_Server 서버 큐가 가득 찼음"
#define _CS_CON_SRV_RCV  "Cuma_Server 수신 완료"


#define _CS_SRV_RCV  "Cuma_Server 수신 에러"

#endif




#endif /* C_Log_hpp */
