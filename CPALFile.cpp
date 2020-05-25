#include "CPALFile.h"
CPALFile::CPALFile()
{
    hOpen = NULL;
    hWrite = NULL;

    m_dwCpkRead = 0x00;
}
CPALFile::~CPALFile()
{
}

int   CPALFile::GetSize() { return ::GetFileSize(hOpen, NULL); }

HANDLE CPALFile::PalOpen(LPCTSTR strOpen, DWORD  dwAccess, DWORD  dwShare, LPSECURITY_ATTRIBUTES lpSAttributes,
    DWORD dwMode, DWORD dwFlag, HANDLE  hTemp)
{
    hOpen = ::CreateFile(strOpen, dwAccess, dwShare, lpSAttributes,
        dwMode, dwFlag, hTemp);
    return hOpen;
}

HANDLE CPALFile::PalOpen(LPCTSTR strOpen)
{
    hOpen = ::CreateFile(strOpen,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    return hOpen;
}

HANDLE CPALFile::PalCreate(LPCTSTR strCreate)
{
    hWrite = CreateFile(strCreate,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hWrite == INVALID_HANDLE_VALUE)
    {
        hWrite = CreateFile(strCreate,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hWrite == INVALID_HANDLE_VALUE)
        {
            return NULL;
        }
    }
    return hWrite;
}

int CPALFile::PalRead(HANDLE hFile, LPVOID pbuf, DWORD dwlen)
{
    int iRe = ReadFile(hFile, pbuf, dwlen, &m_dwCpkRead, NULL);

    int result = iRe == 0x01;

    if (dwlen != m_dwCpkRead) { result = 0; }
    return result;
}

int CPALFile::PalRead(HANDLE hFile, DWORD dwSeek, LPVOID pbuf, DWORD dwlen)
{
    OVERLAPPED oap;
    memset(&oap, 0x00, sizeof(OVERLAPPED));
    oap.Offset = dwSeek;
    int iRe = ReadFile(hFile, (LPBYTE)pbuf, dwlen, &m_dwCpkRead, &oap);

    if (dwlen != m_dwCpkRead) { return m_dwCpkRead; }
    return dwlen;
}

int CPALFile::PalWrite(HANDLE hFile, LPVOID pbuf, DWORD dwlen)
{
    int iRe = ::WriteFile(hFile, pbuf, dwlen, &m_dwCpkRead, NULL);

    int result = iRe == 0x01;

    if (dwlen != m_dwCpkRead) { result = 0; }
    return result;
}

int CPALFile::PalWrite(HANDLE hFile, DWORD dwSeek, LPVOID pbuf, DWORD dwlen)
{
    OVERLAPPED oap;
    memset(&oap, 0x00, sizeof(OVERLAPPED));
    oap.Offset = dwSeek;
    int iRe = ::WriteFile(hFile, (LPBYTE)pbuf, dwlen, &m_dwCpkRead, &oap);

    if (dwlen != m_dwCpkRead) { return m_dwCpkRead; }
    return dwlen;
}