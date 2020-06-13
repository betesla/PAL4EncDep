#pragma once
#include <afx.h>
#include "CRSTEncDec.h"
#include "CPALFile.h"
#include "DataTypes.h"
#include "DBEncDec.h"
#define FILETYPE_FOLDER 0x00003
#define FILETYPE_FILE   0x20001
class CCPKFile : public CRSTEncDec//, public CDBDecEnc
{
public:
    CCPKFile();
    virtual ~CCPKFile();
public:
    CPALFile        file;
    _HEADERINFO     _hdi;
    fileIndex_t*     _plist;
    int             m_iListNum;
    DWORD           dwFileSize;
    LPBYTE          pData;
public:
    fileIndex_t* GetIndexList()
    {
        return (fileIndex_t*)pData;
    }
public:
    int   DecCPKFile(LPCTSTR strOpen, LPCTSTR strSave);
    int   DecCPKIndex(LPCTSTR strOpen);
    int   WriteData(LPCTSTR strpath);
    char* GetDecType(LPVOID pheader);

public:
    int   DecData(char* pdat, int len, LPCTSTR strpath);
    int   SaveFile(int num, char* pdat, int len, LPCTSTR strpath, LPCTSTR strname);
};
