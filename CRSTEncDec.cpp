#include "CRSTEncDec.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char    szVampire[0x10] = "Vampire.C.J at ";
char    szTechn[0x20] = "Softstar Technology (ShangHai)";
char    szCoLtd[0x10] = " Co., Ltd";



CRSTEncDec::CRSTEncDec()
{
    memset(DecKey, 0x00, 0x0100);
}

CRSTEncDec::~CRSTEncDec()
{

}

int       CRSTEncDec::RSTLoadKey(char* key1, char* key2, char* key3)
{
    memset(DecKey, 0x00, 0x100);
    if (key1 == NULL || key2 == NULL || key3 == NULL)
    {
        strcpy(DecKey, szVampire);
        strcat(DecKey, szTechn);
        strcat(DecKey, szCoLtd);
    }
    else {
        strcpy(DecKey, key1);
        strcat(DecKey, key2);
        strcat(DecKey, key3);
    }

    return 0;
}

int       CRSTEncDec::RSTDecrpyt(LPVOID pdat, DWORD  dwnum, LPVOID pkey)
{
    DWORD   dwedx;
    DWORD   dweax;
    DWORD   dwebp;
    DWORD   dwebx = dwnum;
    if (dwebx < 0x02) { return -1; }

    if (pkey == NULL) { pkey = (LPVOID)&DecKey[0]; }

    LPDWORD  pdwdat = (LPDWORD)pdat;
    LPDWORD  pdwkey = (LPDWORD)pkey;

    DWORD    dwesi = pdwdat[0];

    DWORD    esp10 = dwebx;
    DWORD    esp14 = 0x00;

    dweax = (DWORD)floor((double)(52.00f / (float)esp10 + 6.00f)) * 0x9E3779B9;
    esp10 = dweax;

    if (dweax != 0x00)
    {
        do {
            DWORD   dwecx = dweax;
            DWORD   esp1c = (dwecx / 0x04) & 0x03;

            dwecx = dwebx - 0x01;

            if (dwecx > 0x00)
            {
                do {
                    dwedx = pdwdat[dwecx - 0x01];
                    dwebx = dwesi * 0x04;

                    dweax = dwedx;
                    dwebp = dwedx;

                    dweax /= 0x20;
                    dwebx ^= dweax;
                    dweax = dwesi;
                    dweax /= 0x08;
                    dwebp *= 0x10;
                    dweax ^= dwebp;
                    dwebp = esp1c;

                    dwebx += dweax;
                    dweax = dwecx;
                    dweax &= 0x03;
                    dweax ^= dwebp;
                    dwebp = pdwkey[dweax];
                    dweax = esp10;  //esp+10
                    dwebp ^= dwedx;
                    dwedx = dweax;
                    dwedx ^= dwesi;
                    dwesi = pdwdat[dwecx];
                    dwebp += dwedx;
                    dwebx ^= dwebp;
                    dwesi -= dwebx;

                    pdwdat[dwecx] = dwesi;
                    dwecx--;

                } while (dwecx);

                dwebx = dwnum;
            }
            dwebx = pdwdat[dwebx - 1];
            dwedx = dwesi * 0x04;
            dwebp = dwebx;

            dwecx &= 0x03;
            dwebp /= 0x20;
            dwedx ^= dwebp;
            dwebp = dwesi;
            dwebp /= 0x08;
            dwebx *= 0x10;
            dwebp ^= dwebx;

            dwebx = esp1c;
            dwecx ^= dwebx;
            dwedx += dwebp;

            dwecx = pdwkey[dwecx];
            dwebx = dwnum;
            dwebp = pdwdat[dwebx - 1];

            dwecx ^= dwebp;
            dwebp = dweax;
            dwebp ^= dwesi;
            dwesi = pdwdat[0];
            dwecx += dwebp;
            dwedx ^= dwecx;
            dwesi -= dwedx;
            dweax += 0x61C88647;
            pdwdat[0] = dwesi;
            esp10 = dweax;
        } while (dweax);
    }

    return 0;
}


int       CRSTEncDec::RSTDecIndex(LPVOID pbuf, DWORD len)
{
    LPBYTE  pbybuf1 = (LPBYTE)pbuf;
    LPBYTE  pbybuf2 = &pbybuf1[len - 0x1000];

    if (len > 0x2000)
    {
        RSTLoadKey();
        memset(m_chrData[0x00], 0x00, 0x1000);
        memset(m_chrData[0x01], 0x00, 0x1000);

        int num = 0;
        do {
            memcpy(&m_chrData[0x00][num], &pbybuf1[num], 0x08);
            memcpy(&m_chrData[0x01][num + 0x04], &pbybuf2[num - 0x01], 0x08);
            num += 0x08;
        } while (num < 0x1000);

        RSTDecrpyt(&m_chrData[0x00][0x00], 0x0400, DecKey);
        RSTDecrpyt(&m_chrData[0x01][0x04], 0x0400, DecKey);

        int n = 0;

        do {
            memcpy(&pbybuf1[n], &m_chrData[0x00][n], 0x08);
            memcpy(&pbybuf2[n - 0x01], &m_chrData[0x01][n + 0x04], 0x08);

            n++;
            num--;
        } while (num);

        return 1;
    }
    return 0;
}

