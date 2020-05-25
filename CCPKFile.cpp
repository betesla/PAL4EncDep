#include "CCPKFile.h"

CCPKFile::CCPKFile()
{
    _plist = NULL;
    m_iListNum = 0;
    dwFileSize = 0;
    pData = NULL;
    memset(&_hdi, 0x00, sizeof(_HEADER_INFO));
}

CCPKFile::~CCPKFile()
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

int   CCPKFile::DecCPKFile(LPCTSTR strOpen, LPCTSTR strSave)
{
    if (pData == NULL)
    {
        //分配内存
        pData = new BYTE[MAX_FILE_SIZE];
    }
    else { memset(pData, 0x00, MAX_FILE_SIZE); }

    HANDLE  hOpen = file.PalOpen(strOpen);
    if (hOpen == INVALID_HANDLE_VALUE) { return -1; }

    //获取文件大小
    dwFileSize = ::GetFileSize(hOpen, NULL);
    printf("FileSize is:%d\n", dwFileSize);
    //读取索引头
    DecCPKIndex(strOpen);
    printf("File number total:%d\n", _hdi._06dwFileNum);
    CString ss = strSave;
    ss += "index.bin";
    HFILE   hf = ::_lcreat(ss, NULL);
    ::_lwrite(hf, (LPCTSTR)pData, MAX_FILE_SIZE);
    ::_lclose(hf);

    //写文件:
    WriteData(strSave);

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

int    CCPKFile::DecCPKIndex(LPCTSTR strOpen)
{
    memset(&_hdi, 0x00, sizeof(_HEADER_INFO));
    file.PalRead(file.hOpen, &_hdi, sizeof(_HEADER_INFO));

    m_iListNum = _hdi._06dwFileNum;//CPK中的文件列表

    if (_hdi._01dwLogon == FILE_LOGON)
    {
        if (file.PalRead(file.hOpen, pData, _hdi._05dwLenSub * 0x20) != 0)
        {
            int re = RSTDecIndex(pData, MAX_FILE_SIZE);
            _plist = GetIndexList();
            return re;
        }
    }

    return 0;
}

char* CCPKFile::GetDecType(LPVOID pheader)
{
    DWORD  dwType = (*(LPDWORD)pheader & 0x00FFFFFF);

    if (strcmp((char*)&dwType, "BIK") == 0x00) { return (char*)"BIK"; }

    return (char*)"unk";
}

int     CCPKFile::WriteData(LPCTSTR strpath)
{
    if (dwFileSize <= 0x100000) { return -1; }
    int     size = dwFileSize - 0x100000;
    int     rsize = 0x100000;
    int     seek = 0x100000;

    char    ss[MAX_PATH] = { 0 };

    printf("分配内存 ...!+'\n'", 0x01);
    LPBYTE  pbuf = new BYTE[size];

    int     num = size / 0x100000;
    if (size < 0x100000) { rsize = size; }
    if ((size % 0x100000) > 0x00) { num++; }

    //循环读取文件
    do {
        int iRe = file.PalRead(file.hOpen, seek, pbuf + (seek - 0x100000), rsize);
        seek += rsize;

        wsprintf(ss, "(%d)装载文件: %d / %d", num, seek - 0x100000, size);
        printf(ss + '\r', 0x01);
        //theApp.m_Dlg->m_pgsLog.SetPos(((seek / 0x1000) * 100) / (dwFileSize / 0x1000));

        if ((dwFileSize - seek) < 0x100000) { rsize = dwFileSize - seek; }
        num--;
    } while (num);

    printf("读取完成,解密文件 ...!\n", 0x01);

    //保存文件
    DecData((char*)pbuf, size, strpath);

    delete[] pbuf;

    return 0;
}

int     CCPKFile::DecData(char* pdat, int len, LPCTSTR strpath)
{
    char   ss[MAX_PATH] = { 0 };
    strcpy(ss, strpath);

    for (int num = 0; num < m_iListNum; num++)
    {
        int   size = _plist[num]._05dwLenght1;
        int   seek = _plist[num]._04dwSeek - 0x100000;
        char* psrc = pdat + seek;

        if (_plist[num]._05dwLenght1 == 0x00 && _plist[num]._06dwLenght2 == 0x00)
        {
            wsprintf(ss, "%s%s", strpath, (char*)(psrc + size));
            ::CreateDirectory(ss, NULL);
            //strcpy(ss, strpath);
            continue;
        }

        DWORD   dstlen = 0x00;
        if (_plist[num]._06dwLenght2 <= 0x1000) { dstlen = 0x1000; }
        if (_plist[num]._06dwLenght2 > 0x1000) { dstlen = (_plist[num]._06dwLenght2 / 0x1000) * 0x1000 + 0x1000; }

        char* pdst = new char[dstlen];

        if (_plist[num]._05dwLenght1 != _plist[num]._06dwLenght2)
        {
            //printf("This part of code is to be fixed later\n");
            int iRe = DBDecrpyt((const unsigned char*)psrc, size, (unsigned char*)pdst, &dstlen, 0x00);
            SaveFile(num, pdst, dstlen, ss, (char*)(psrc + size));
            //SaveFile(num, psrc, size, ss, (char*)(psrc + size));
        }
        else {
            SaveFile(num, psrc, size, ss, (char*)(psrc + size));
        }

        delete[] pdst;
    }
    return 0;
}

int     CCPKFile::SaveFile(int num, char* pdat, int len, LPCTSTR strpath, LPCTSTR strname)
{
    char    ss[MAX_PATH] = { 0 };
    char    path[MAX_PATH] = { 0 };

    wsprintf(path, "%s\\%s", strpath, strname);
    if (file.PalCreate(path) < 0)
    {
        wsprintf(ss, "(%d)创建文件[%02X]<0x%08X>:%s  ...创建错误!\n",
            num, _plist[num]._07dwNumber, _plist[num]._04dwSeek, strname);

        printf(ss + '\n');
        return 0;
    }

    wsprintf(ss, "(%d)创建文件[%02X]<0x%08X>:%s  ...创建成功!\n",
        num, _plist[num]._07dwNumber, _plist[num]._04dwSeek, strname);
    printf(ss + '\n');

    if (len > 0x00)
    {
        int wlen = 0x1000;
        int size = 0x00;
        int n = len / 0x1000;
        if (len < 0x1000) { wlen = len; }
        if ((len % 0x1000) > 0x00) { n++; }

        do {
            file.PalWrite(file.hWrite, pdat + size, wlen);
            size += wlen;

            int p = 100;
            if (size > 0x00) {
                if ((size / 0x1000) > 0x00) { p = ((size / 0x1000) * 100) / (len / 0x1000); }
            }

            wsprintf(ss, "(%d)%s: (%d/%d) [%s%d]\r",
                num, strname, size, len, "%", p);
            printf(ss);
            //theApp.m_Dlg->m_pgsLog.SetPos(p);

            if ((len - size) < 0x1000 && (len - size) > 0x00) { wlen = len - size; }
            if ((len - size) < 0x00) { break; }
            n--;
        } while (n);
    }

    file.PalClose(file.hWrite); file.hWrite = NULL;
    return 1;
}