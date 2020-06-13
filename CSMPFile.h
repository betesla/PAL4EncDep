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
    fileIndex_t* _plist;
    int            m_iListNum;
    DWORD           dwFileSize;
    LPBYTE          pData;
public:
    fileIndex_t* GetIndexList()
    {
        return (fileIndex_t*)pData;
    }
public:
    int   DecSMPFile(LPCTSTR strOpen, LPCTSTR strSave, LPCTSTR name = NULL);
    int   DecSMPIndex(LPCTSTR strOpen);
    int   WriteData(LPCTSTR strpath, LPCTSTR name);
    char* GetDecType(LPVOID pheader);
public:
    int   SaveFile(int num, char* name);
};