#pragma once
#include <afx.h>
#define MAX_FILE_SIZE 0xA00000//最大文件大小10MB
//DWORD   FILE_LOGON = 0x1A545352; //"RST"
#define   FILE_LOGON 0x1A545352UL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

typedef struct  _PACKFILE_HEADER_INFO {
    DWORD   _01dwLogon; //0x1A545352 "RST"
    DWORD   _02dwUnk; //04
    DWORD   _03dwIndexSeek; //08
    DWORD   _04dwDataSeek; //0C
    DWORD   _05dwLenSub; //10
    DWORD   _06dwFileNum; //14
    DWORD   _07dwUnk; //18
    DWORD   _08dwUnk; //1C
    DWORD   _09dwUnk; //20
    DWORD   _0AdwUnk; //24
    DWORD   _0BdwUnk; //28
    DWORD   _0CdwFileSize; //2C
    DWORD   _10dwUnk[0x14]; //30
}_HEADER_INFO, _HEADERINFO;
//-------------------------------------------------------------------------------------------------------------
typedef struct  _RST_DATA_INDEXLIST {
    DWORD   _01dwUnk;
    DWORD   _02dwUnk;
    DWORD   _03dwUnk;
    DWORD   _04dwSeek;     //文件位置
    DWORD   _05dwLenght1;  //加密后长度
    DWORD   _06dwLenght2;  //解密后长度
    DWORD   _07dwNumber;
    DWORD   _08dwEnd;
}_DATA_LIST;