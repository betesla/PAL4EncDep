#pragma once
#include <afx.h>
#define MAX_FILE_SIZE 0x100000//����ļ���С10MB
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
    DWORD   _03dwIndexSeek; //08 ����ƫ��ֵ
    DWORD   _04dwDataSeek; //0C ����ƫ��ֵ
    DWORD   _05dwLenSub; //10 Rev2 //00,80,00,00//�����������32768//����ļ�����
    DWORD   _06dwFileNum; //14 //��¼��
    DWORD   _07dwUnk; //18  //RevZ
    DWORD   _08dwUnk; //1C //RevVF1
    DWORD   _09dwUnk; //20 blokcNum2
    DWORD   _0AdwUnk; //24 00800000
    DWORD   _0BdwUnk; //28 0000000
    DWORD   _0CdwFileSize; //����CPK�ĳ���
    DWORD   _10dwUnk[0x14]; //30
}_HEADER_INFO, _HEADERINFO;

//-------------------------------------------------------------------------------------------------------------
typedef struct  _RST_DATA_INDEXLIST {
    DWORD   _01dwUnk;
    DWORD   _02dwUnk;
    DWORD   _03dwUnk;
    DWORD   _04dwSeek;     //�ļ�λ��
    DWORD   _05dwLenght1;  //���ܺ󳤶�
    DWORD   _06dwLenght2;  //���ܺ󳤶�
    DWORD   _07dwNumber;
    DWORD   _08dwEnd;
}_DATA_LIST;

typedef struct  _FILE_INDEX {
    DWORD   VID;        //01��¼ID
    DWORD   Rev2;       //02�ڵ�����
    DWORD   ParentVID;  //03�ϼ�ID
    DWORD   Offset;     //04�ļ�ƫ��λ��
    DWORD   LzoSize;    //05ѹ����ĳ���
    DWORD   FileSize;   //06ԭʼ�ļ�����
    DWORD   RevFnLen;   //07���Ƴ���
    DWORD   _08dwEnd;
}fileIndex_t;