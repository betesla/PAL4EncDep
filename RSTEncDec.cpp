// RSTEncDec.cpp: implementation of the CRSTEncDec class.
//
//////////////////////////////////////////////////////////////////////
#include <cmath>



//---------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------




/*

int     CCPKFile::WriteData(LPCTSTR strpath)
{
    char    path[MAX_PATH] = {0};
    char    ss  [MAX_PATH] = {0};

    for(int num =0; num < m_iListNum; num++)
    {
        BYTE    pdat[0x10]={0};

        int iRe=file.PalRead(file.hOpen,_plist[num]._04dwSeek,pdat,0x10); if(iRe<0){ break; }

        wsprintf(path,"%sPAL4_Dec_%02X.%s",strpath,num,GetDecType(pdat));
        if(file.PalCreate(path)<0)
        {
            wsprintf(ss,"(%d)创建文件[%02X]<0x%08X>:PAL4_Dec_%02X.%s  ...创建错误!",
                num,_plist[num]._07dwNumber,_plist[num]._04dwSeek,num,GetDecType(pdat));

            printf(ss);
            continue;
        }
        wsprintf(ss,"(%d)创建文件[%02X]<0x%08X>:PAL4_Dec_%02X.%s  ...创建成功!",
                num,_plist[num]._07dwNumber,_plist[num]._04dwSeek,num,GetDecType(pdat));

        printf(ss);

        SaveFile(num,GetDecType(pdat));

        file.PalClose(file.hWrite); file.hWrite=NULL;

    }

    return 0;
}

int     CCPKFile::SaveFile(int num,char *sztype)
{
    char    pdat[0x1000]   = {0};
    char    ss  [MAX_PATH] = {0};
    int   size        =_plist[num]._05dwLenght1;
    int   seek        =_plist[num]._04dwSeek   ;
    int   len         =0;

    if(size>=0x1000){ len=0x1000;}else{len=size;}

    theApp.m_Dlg->m_pgsLog.SetPos(0);
    int  wsize=0;

    do{
        int  iRe=file.PalRead(file.hOpen,seek,pdat,len);if(iRe<0){break;}
        file.PalWrite (file.hWrite,pdat,iRe);
        seek  += len;
        wsize += len;

        int p=100;
        if(wsize > 0x00) {
            if((wsize/0x1000)>0x00){ p= ((wsize /0x1000) *100)/ (size /0x1000);}
        }

        wsprintf(ss,"PAL4_Dec_%02X.%s ->处理:%d/%d [%s%d]",
                num,sztype,wsize ,_plist[num]._05dwLenght1,"%",
                 p);

        printf(ss,0x01);
        theApp.m_Dlg->m_pgsLog.SetPos( p );
        if((size -wsize) < 0x1000){ len=size -wsize ;}
    }while(len>0);

    wsprintf(ss,"(%d)处理成功:PAL4_Dec_%02X.%s ->[处理%dByte/原始%dByte]!",
        num,num,sztype,wsize,_plist[num]._05dwLenght1);
    printf(ss,0x00);

    return 0;
}
*/



//----------------------------------------------------------------------------------------------------------------------


