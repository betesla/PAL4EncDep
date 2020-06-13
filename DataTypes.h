#pragma once
#include <afx.h>
#define MAX_FILE_SIZE 0x100000//最大文件大小10MB
//DWORD   FILE_LOGON = 0x1A545352; //"RST"
#define   FILE_LOGON 0x1A545352UL

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

typedef struct  _PACKFILE_HEADER_INFO {
    DWORD   _01dwLogon; //0x1A545352 "RST>"
    DWORD   _02dwUnk; //04
    DWORD   _03dwIndexSeek; //08 索引偏移值
    DWORD   _04dwDataSeek; //0C 数据偏移值
    DWORD   _05dwLenSub; //10 Rev2 //00,80,00,00//最大索引数量32768//最大文件数量
    DWORD   _06dwFileNum; //14 //记录数
    DWORD   _07dwUnk; //18  //RevZ
    DWORD   _08dwUnk; //1C //RevVF1
    DWORD   _09dwUnk; //20 blokcNum2
    DWORD   _0AdwUnk; //24 00800000
    DWORD   _0BdwUnk; //28 0000000
    DWORD   _0CdwFileSize; //整个CPK的长度
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

typedef struct  _FILE_INDEX {
    DWORD   VID;        //01记录ID
    DWORD   Rev2;       //02节点种类
    DWORD   ParentVID;  //03上级ID
    DWORD   Offset;     //04文件偏移位置
    DWORD   LzoSize;    //05压缩后的长度
    DWORD   FileSize;   //06原始文件长度
    DWORD   RevFnLen;   //07名称长度
    DWORD   _08dwEnd;
}fileIndex_t;