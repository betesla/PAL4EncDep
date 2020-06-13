#include "CRSTEncDec.h"
//#include "stdint.h"

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))
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
/*!
* @brief �˴�Ӧ������ȡ���ɽ�4����cpkʱʹ�õĳ���ξ���������д���Ľ��ܳ���
* ���õļ��ܷ���ӦΪxxtea
*/
int  CRSTEncDec::RSTDecrpyt(LPVOID pdat, DWORD  n, LPVOID pkey)
{
    uint32_t y, z, sum;
    unsigned p, rounds, e;

    DWORD   dwedx;
    DWORD   dweax;
    DWORD   dwebp;
    DWORD   dwebx = n;
    if (n < 0x02) { return -1; }

    rounds = (6 + 52 / n);

    if (pkey == NULL) { pkey = (LPVOID)&DecKey[0]; }

    LPDWORD  pdwdat = (LPDWORD)pdat;
    LPDWORD  pdwkey = (LPDWORD)pkey;

    DWORD    dwesi = pdwdat[0];

    DWORD    esp10 = n;
    DWORD    esp14 = 0x00;

    dweax = rounds * DELTA;
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
                    dwebx = dwesi << 2;//y<<2

                    dweax = dwedx;
                    dwebp = dwedx;

                    dweax >>= 5;//z>>5
                    dwebx ^= dweax;
                    dweax = dwesi;
                    dweax <<= 3;
                    dwebp >>=4;
                    dweax ^= dwebp;
                    dwebp = esp1c;

                    dwebx += dweax;
                    dweax = dwecx;
                    dweax &= 3;//key[(p&3)^e]
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

                dwebx = n;
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
            dwebx = n;
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
void CRSTEncDec::btea(uint32_t* v, int n, uint32_t const key[4]){
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1) {          /* Coding Part */
        rounds = 6 + 52 / n;
        sum = 0;
        z = v[n - 1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p = 0; p < n - 1; p++) {
                y = v[p + 1];
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n - 1] += MX;
        } while (--rounds);
    }
    else if (n < -1) {  /* Decoding Part */
        n = -n;
        rounds = 6 + 52 / n;
        sum = rounds * DELTA;
        y = v[0];
        do {
            e = (sum >> 2) & 3;
            for (p = n - 1; p > 0; p--) {
                z = v[p - 1];
                y = v[p] -= MX;
            }
            z = v[n - 1];
            y = v[0] -= MX;
        } while ((sum -= DELTA) != 0);
    }
}

int       CRSTEncDec::RSTDecIndex(LPVOID pbuf, DWORD len)
{
    //�������ܴ�С����0x2000
    LPBYTE  pbybuf1 = (LPBYTE)pbuf;//Pbuf1ָ�򻺳���1��ʼ����
    LPBYTE  pbybuf2 = &pbybuf1[len - 0x1000];//Pbuf2ָ�򻺳�������ǰ0x1000λ��
    uint8_t* pbuf3 = (uint8_t *)malloc(0x1000);//�����㷨�Ա��ڴ�
    if (len > 0x2000)
    {
        RSTLoadKey();
        memset(m_chrData[0x00], 0x00, 0x1000);//��ջ�����1
        memset(m_chrData[0x01], 0x00, 0x1000);//��ջ�����2
        memset(pbuf3, 0x00, 0x1000);
        int num = 0;
        do {
            memcpy(&m_chrData[0x00][num], &pbybuf1[num], 0x08);//��pbuf1������������1�У���С0x1000
            memcpy(pbuf3+num, &pbybuf1[num], 0x08);
            memcpy(&m_chrData[0x01][num + 0x04], &pbybuf2[num - 0x01], 0x08);//��pbuf2������������2�У�����ƫ��4���ֽڣ���ǰ4���ֽڶ�Ϊ0����С0x1000���˴�Ӧ����Index�����β��0x1000���ֽ�
            num += 0x08;
        } while (num < 0x1000);

        RSTDecrpyt(&m_chrData[0x00][0x00], 0x0400, DecKey);
        //RSTDecrpyt(&m_chrData[0x01][0x04], 0x0400, DecKey);

        btea((uint32_t *)pbuf3, 0x0400,(const uint32_t *) DecKey);
        //btea((uint32_t *)&m_chrData[0x01][0x04], 0x0400,(const uint32_t *) DecKey);

        int n = 0;
        //���ܺ�����ݿ�����ԭλ�á����������������CRC��Ϊ�����ļ����ݵĻ��ƣ��˴�˫������������Ӧ����Ϊ�˷����ǰ�����ߴӺ���ǰ�����ļ��������ٶ�
        //����֪�����乤�����ƣ����ս�������ǿ��Դ��ڴ����֣������������ʵ��ڴ����ݣ�����Խ�����ص��ļ��ض�λ������ϣ�����ļ�λ��
        do {
            memcpy(&pbybuf1[n], &m_chrData[0x00][n], 0x08);
            memcpy(&pbybuf2[n - 0x01], &m_chrData[0x01][n + 0x04], 0x08);

            n++;
            num--;
        } while (num);
        delete[] pbuf3;
        return 1;
    }
    return 0;
}

