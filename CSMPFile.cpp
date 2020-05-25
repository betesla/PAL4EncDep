//----------------------------------------------------------------------------------------------------------------------
#include "CSMPFile.h"

CSMPFile::CSMPFile()
{
    _plist = NULL;
    m_iListNum = 0;
    dwFileSize = 0;
    pData = NULL;
    memset(&_hdi, 0x00, sizeof(_HEADER_INFO));
}

CSMPFile::~CSMPFile()
{
    if (pData != NULL)
    {
        //删除内存分配
        delete[] pData; pData = NULL;
    }

    if (file.hOpen != NULL)
    {
        file.PalClose(file.hOpen);
        file.hOpen = NULL;
    }
}

int   CSMPFile::DecSMPFile(LPCTSTR strOpen, LPCTSTR strSave, LPCTSTR name)
{
    if (pData == NULL)
    {
        //分配内存
        pData = new BYTE[0x100000];
    }
    else { memset(pData, 0x00, 0x100000); }

    HANDLE  hOpen = file.PalOpen(strOpen);
    if (hOpen == INVALID_HANDLE_VALUE) { return -1; }

    //获取文件大小
    dwFileSize = ::GetFileSize(hOpen, NULL);

    //读取索引头
    DecSMPIndex(strOpen);

    if (name == NULL)
    {
        //写文件:
        WriteData(strSave, "PAL4_DEC");
    }
    else { WriteData(strSave, name); }


    //清除句柄:
    if (file.hOpen != NULL)
    {
        file.PalClose(file.hOpen);
        file.hOpen = NULL;
    }

    if (file.hWrite != NULL)
    {
        file.PalClose(file.hWrite);
        file.hWrite = NULL;
    }

    delete[] pData;

    return 0;
}

int    CSMPFile::DecSMPIndex(LPCTSTR strOpen)
{
    memset(&_hdi, 0x00, sizeof(_HEADER_INFO));
    file.PalRead(file.hOpen, &_hdi, sizeof(_HEADER_INFO));

    m_iListNum = _hdi._06dwFileNum;

    if (_hdi._01dwLogon == FILE_LOGON)
    {
        if (file.PalRead(file.hOpen, pData, _hdi._05dwLenSub * 0x20) != 0)
        {
            int re = RSTDecIndex(pData, 0x100000);
            _plist = GetIndexList();
            return re;
        }
    }

    return 0;
}

char* CSMPFile::GetDecType(LPVOID pheader)
{
    DWORD  dwType = (*(LPDWORD)pheader & 0x00FFFFFF);

    //if(strcmp((char*)&dwType,"BIK")==0x00){ return "BIK";}

    return (char*)"MP3";
}

int     CSMPFile::WriteData(LPCTSTR strpath, LPCTSTR name)
{
    char    path[MAX_PATH] = { 0 };
    char    ss[MAX_PATH] = { 0 };
    char    cc[MAX_PATH] = { 0 };

    RSTLoadKey();

    for (int num = 0; num < m_iListNum; num++)
    {
        BYTE    pdat[0x10] = { 0 };

        int iRe = file.PalRead(file.hOpen, _plist[num]._04dwSeek, pdat, 0x10); if (iRe < 0) { break; }

        if (num == 0) { wsprintf(cc, "%s.%s", name, GetDecType(pdat)); }
        else { wsprintf(cc, "%s_%02X.%s", name, num, GetDecType(pdat)); }

        wsprintf(path, "%s%s", strpath, cc);
        if (file.PalCreate(path) < 0)
        {
            wsprintf(ss, "(%d)创建文件[%02X]<0x%08X>:%s  ...创建错误!",
                num, _plist[num]._07dwNumber, _plist[num]._04dwSeek, cc);

            printf(ss + '\n');
            continue;
        }
        wsprintf(ss, "(%d)创建文件[%02X]<0x%08X>:%s  ...创建成功!",
            num, _plist[num]._07dwNumber, _plist[num]._04dwSeek, cc);

        printf(ss + '\n');

        SaveFile(num, cc);

        file.PalClose(file.hWrite); file.hWrite = NULL;

    }

    return 0;
}

int     CSMPFile::SaveFile(int num, char* name)
{
    char  ss[MAX_PATH] = { 0 };
    int   size = _plist[num]._05dwLenght1;
    int   seek = _plist[num]._04dwSeek;
    int   len = 0;

    char* pdat = new char[size];
    memset(pdat, 0x00, size);

    if (size >= 0x1000) { len = 0x1000; }
    else { len = size; }

    //theApp.m_Dlg->m_pgsLog.SetPos(0);
    int  wsize = 0;
    int  iRe = file.PalRead(file.hOpen, seek, pdat, size);
    if (iRe < 0) { return -1; }

    RSTDecrpyt(pdat, size / 4);

    do {
        file.PalWrite(file.hWrite, &pdat[wsize], len);
        wsize += len;

        wsprintf(ss, "%s ->处理:%d/%d [%s%d]",
            name, wsize, _plist[num]._05dwLenght1, "%",
            ((wsize / 0x1000) * 100) / (size / 0x1000));

        printf(ss + '\n', 0x01);
        //theApp.m_Dlg->m_pgsLog.SetPos(((wsize / 0x1000) * 100) / (size / 0x1000));
        if ((size - wsize) < 0x1000) { len = size - wsize; }
    } while (len > 0);

    delete[] pdat;
    wsprintf(ss, "(%d)处理成功:%s ->[处理%dByte/原始%dByte]!",
        num, name, wsize, _plist[num]._05dwLenght1);
    printf(ss + '\n', 0x00);

    return 0;
}