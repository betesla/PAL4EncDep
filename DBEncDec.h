#pragma once
#include "minilzo.h"
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
/* decompression */
LZO_EXTERN(int)
DBDecrpyt(const lzo_bytep src, lzo_uint  src_len,
    lzo_bytep dst, lzo_uintp dst_len,
    lzo_voidp wrkmem /* NOT USED */);