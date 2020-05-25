#pragma once
#include <afx.h>
class CPALFile
{
public:
    CPALFile();
    virtual ~CPALFile();
public:
    HANDLE  hOpen;
    HANDLE  hWrite;
    DWORD   m_dwCpkRead;
public:
    int       GetSize();
public:
    HANDLE    PalOpen(LPCTSTR strOpen);
    HANDLE    PalOpen(LPCTSTR strOpen, DWORD dwAccess, DWORD  dwShare, LPSECURITY_ATTRIBUTES lpSAttributes,
        DWORD dwMode, DWORD dwFlag, HANDLE  hTemp);
    HANDLE    PalCreate(LPCTSTR strCreate);
    int       PalRead(HANDLE hFile, LPVOID pbuf, DWORD dwlen);
    int       PalRead(HANDLE hFile, DWORD dwSeek, LPVOID pbuf, DWORD dwlen);
    int       PalWrite(HANDLE hFile, LPVOID pbuf, DWORD dwlen);
    int       PalWrite(HANDLE hFile, DWORD dwSeek, LPVOID pbuf, DWORD dwlen);
    void      PalClose(HANDLE hFile) { ::CloseHandle(hFile); hFile = NULL; }
};