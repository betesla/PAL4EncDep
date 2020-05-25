#pragma once
#include <afx.h>
#include "CRSTEncDec.h"
#include "CPALFile.h"
#include "DataTypes.h"
class CSMPFile : public CRSTEncDec
{
public:
    CSMPFile();
    virtual ~CSMPFile();
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
    int   DecSMPFile(LPCTSTR strOpen, LPCTSTR strSave, LPCTSTR name = NULL);
    int   DecSMPIndex(LPCTSTR strOpen);
    int   WriteData(LPCTSTR strpath, LPCTSTR name);
    char* GetDecType(LPVOID pheader);
public:
    int   SaveFile(int num, char* name);
};