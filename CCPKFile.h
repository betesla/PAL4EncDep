#pragma once
#include <afx.h>
#include "CRSTEncDec.h"
#include "CPALFile.h"
#include "DataTypes.h"
#include "DBEncDec.h"
class CCPKFile : public CRSTEncDec//, public CDBDecEnc
{
public:
    CCPKFile();
    virtual ~CCPKFile();
public:
    CPALFile        file;
    _HEADERINFO     _hdi;
    _DATA_LIST* _plist;
    int            m_iListNum;
    DWORD           dwFileSize;
    LPBYTE          pData;
public:
    _DATA_LIST* GetIndexList()
    {
        return (_DATA_LIST*)pData;
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
