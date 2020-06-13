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
        //ɾ���ڴ����
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
        //�����ڴ�
        pData = new BYTE[MAX_FILE_SIZE];
    }
    else { memset(pData, 0x00, MAX_FILE_SIZE); }

    HANDLE  hOpen = file.PalOpen(strOpen);
    if (hOpen == INVALID_HANDLE_VALUE) { return -1; }

    //��ȡ�ļ���С
    dwFileSize = ::GetFileSize(hOpen, NULL);
    printf("FileSize is:%d\n", dwFileSize);
    //��ȡ����ͷ
    DecCPKIndex(strOpen);
    printf("File number total:%d\n", _hdi._06dwFileNum);
    CString ss = strSave;
    ss += "index.bin";
    HFILE   hf = ::_lcreat(ss, NULL);
    ::_lwrite(hf, (LPCTSTR)pData, MAX_FILE_SIZE);
    ::_lclose(hf);

    //д�ļ�:
    WriteData(strSave);

    //������:
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

    m_iListNum = _hdi._06dwFileNum;//CPK�е��ļ��б�

    if (_hdi._01dwLogon == FILE_LOGON)
    {
        if (file.PalRead(file.hOpen, pData, _hdi._05dwLenSub * 0x20) != 0)//����ļ���������Ϊ0x8000 ��32768��0x20Ϊ�����Ĵ�С�����Դ˴�Ϊ��ȡ
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

    printf("�����ڴ� ...!+'\n'", 0x01);
    LPBYTE  pbuf = new BYTE[size];

    int     num = size / 0x100000;
    if (size < 0x100000) { rsize = size; }
    if ((size % 0x100000) > 0x00) { num++; }

    //ѭ����ȡ�ļ�
    do {
        int iRe = file.PalRead(file.hOpen, seek, pbuf + (seek - 0x100000), rsize);
        seek += rsize;

        wsprintf(ss, "(%d)װ���ļ�: %d / %d", num, seek - 0x100000, size);
        printf(ss);
        //theApp.m_Dlg->m_pgsLog.SetPos(((seek / 0x1000) * 100) / (dwFileSize / 0x1000));

        if ((dwFileSize - seek) < 0x100000) { rsize = dwFileSize - seek; }
        num--;
    } while (num);

    printf("��ȡ���,�����ļ� ...!\n", 0x01);

    //�����ļ�
    DecData((char*)pbuf, size, strpath);

    delete[] pbuf;

    return 0;
}

int     CCPKFile::DecData(char* pdat, int len, LPCTSTR strpath)
{
    char   ss[MAX_PATH] = { 0 };
    strcpy(ss, strpath);

    for (int num = 0; num < m_iListNum; num++)//ѭ����ȡÿ���ļ�
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
            SaveFile(num, pdst, dstlen, ss, (char*)(psrc + size));//< �洢���ܺ���ļ�
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
        wsprintf(ss, "(%d)�����ļ�[%02X]<0x%08X>:%s  ...��������!\n",
            num, _plist[num].RevFnLen, _plist[num].Offset, strname);

        printf(ss + '\n');
        return 0;
    }

    wsprintf(ss, "(%d)�����ļ�[%02X]<0x%08X>:%s  ...�����ɹ�!\n",
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