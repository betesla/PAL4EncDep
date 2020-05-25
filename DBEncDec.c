#include "DBEncDec.h"
DBDecrpyt(const lzo_bytep src, lzo_uint  src_len,
    lzo_bytep dst, lzo_uintp dst_len,
    lzo_voidp wrkmem /* NOT USED */){
    /*
    *step1:initialize the lzo library
    */
    if (lzo_init() != LZO_E_OK) {
        printf("lzo_init() failed\n");
        return 3;
    }
    int outLen = 0;
    int r = lzo1x_decompress(src, src_len, dst,&outLen, wrkmem);
    *dst_len = outLen;
    if (r == LZO_E_OK && dst_len == outLen) {
        printf("Decompressed %lu bytes to %lu bytes\n",src_len,outLen);
    }
    else
    {
        printf("internal error-decompression failed\n");
    }
    return 0;    
}