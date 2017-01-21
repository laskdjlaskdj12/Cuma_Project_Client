#
# Makefile
# Cuma_Project
# Cuma_Client
#
# Create by laskdjlaskdj12@gmail.com.
# Copyright (c) 2016 InhoeKu LP. All right Reserved.
#
# LICENCED by JSONCPP (C) 2016 by cdunn2001 All right Reserved.
#


cmake_minimum_required (VERSION 2.6)
project (Cuma_Client_Remake)
set(CMAKE_MACOSX_RPATH 1)

# add Current_directory
include_directories ("${PROJECT_SOURCE_DIR}")

# add Sub_directory
include_directories ("${PROJECT_SOURCE_DIR}/Cuma_Client")

# jsoncpp header include
include_directories("${CMAKE_SOURCE_DIR}/dist"
    "${CMAKE_SOURCE_DIR}/dist/json")

add_library (Json_obj SHARED "${PROJECT_SOURCE_DIR}/dist/jsoncpp.cpp")

# 프레임
add_library (Frame_obj SHARED "${PROJECT_SOURCE_DIR}/Cuma_Frame.cpp")
target_link_libraries(Frame_obj Json_obj)

#소켓
add_library (Socket_obj SHARED "${PROJECT_SOURCE_DIR}/Cuma_Client/Cli_Sck.cpp")

#클라이언트
add_library (Client_obj SHARED "${PROJECT_SOURCE_DIR}/Cuma_Client/Cuma_client.cpp")
target_link_libraries (Client_obj Socket_obj Frame_obj)

#main 실행_executive_link
add_executable(Cuma_Client_Remake "${PROJECT_SOURCE_DIR}/Cuma_Client/main.cpp")

#타겟 링크 라이브러리
target_link_libraries (Cuma_Client_Remake Client_obj)
