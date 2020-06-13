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

    //delete[] pData;

    return 0;
}

int    CCPKFile::DecCPKIndex(LPCTSTR strOpen)
{
    memset(&_hdi, 0x00, sizeof(_HEADER_INFO));
    file.PalRead(file.hOpen, &_hdi, sizeof(_HEADER_INFO));

    m_iListNum = _hdi._06dwFileNum;//CPK中的文件列表

    if (_hdi._01dwLogon == FILE_LOGON)
    {
        if (file.PalRead(file.hOpen, pData, _hdi._05dwLenSub * 0x20) != 0)//最大文件索引数量为0x8000 即32768，0x20为索引的大小，所以此处为读取
        {
            int re = RSTDecIndex(pData, _hdi._05dwLenSub * 0x20);
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
        printf(ss);
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

    for (int num = 0; num < m_iListNum; num++)//循环读取每个文件
    {
        //TestMethod start
        //printf("File type:0x%x,name:%s\n",_plist[num].Rev2,pdat-0x100000+_plist[num].Offset);
        
        //continue;
        //TestMethod end
        int   size = _plist[num].LzoSize;
        int   seek = _plist[num].Offset - 0x100000;
        char* psrc = pdat + seek;

        if (_plist[num].Rev2==FILETYPE_FOLDER)
        {
            wsprintf(ss, "%s%s", strpath, (char*)(psrc + size));
            ::CreateDirectory(ss, NULL);
            strcpy(ss, strpath);
            continue;
        }
        int parentSize = 0;
        int parentSeek = 0;
        char* parentName = 0;
        if (_plist[num].ParentVID != 0) {
            
            for (size_t i = 0; i < m_iListNum; i++){
                if (_plist[i].VID == _plist[num].ParentVID) {
                    parentSize = _plist[i].LzoSize;
                    parentSeek = _plist[i].Offset - 0x100000;
                    parentName = pdat + parentSeek;
                    printf("ParentName is:%s\n",parentName+parentSize);
                    break;
                }
            }
        }
        wsprintf(ss,"%s%s\\",ss,parentName);
        DWORD   dstlen = 0x00;
        if (_plist[num].FileSize <= 0x1000) { dstlen = 0x1000; }
        if (_plist[num].FileSize > 0x1000) { dstlen = (_plist[num].FileSize / 0x1000) * 0x1000 + 0x1000; }

        char* pdst = new char[dstlen];

        printf("(%d/%d),05dwLength1:%d,06dwLength2:%d,dstlen:%d\n",num, m_iListNum,_plist[num].LzoSize,_plist[num].FileSize,dstlen);

        if (_plist[num].LzoSize != _plist[num].FileSize)
        {
            //printf("This part of code is to be fixed later\n");
            int iRe = DBDecrpyt((const unsigned char*)psrc, size, (unsigned char*)pdst, &dstlen, 0x00);
            SaveFile(num, pdst, dstlen, ss, (char*)(psrc + size));//< 存储解密后的文件
            //SaveFile(num, psrc, size, ss, (char*)(psrc + size));
        }
        else {
            SaveFile(num, psrc, size, ss, (char*)(psrc + size));
        }
        strcpy(ss, strpath);
        delete[] pdst;
        printf("Delete success\n");
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
            num, _plist[num].RevFnLen, _plist[num].Offset, strname);

        printf(ss + '\n');
        return 0;
    }

    wsprintf(ss, "(%d)创建文件[%02X]<0x%08X>:%s  ...创建成功!\n",
        num, _plist[num].RevFnLen, _plist[num].Offset, strname);
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