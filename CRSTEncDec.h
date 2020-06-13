#pragma once
#include <afx.h>
#include "stdint.h"
class CRSTEncDec
{
public:
    CRSTEncDec();
    virtual ~CRSTEncDec();
private:
    char      DecKey[0x0100];
public:
    char      m_chrData[0x02][0x1000];
public:
    int       RSTLoadKey(char* key1 = NULL, char* key2 = NULL, char* key3 = NULL);
    int       RSTDecrpyt(LPVOID pdat, DWORD  dwnum, LPVOID pkey = NULL);
    void      btea(uint32_t* v, int n, uint32_t const key[4]);
    int       RSTDecIndex(LPVOID pbuf, DWORD len);
};