#include "include/stdint.h"

uint32_t vsprintf(char *dst, char* fmt, __builtin_va_list args)
{
    int64_t arg;
    int32_t len, sign, i;
    char *p, *orig=dst, tmpstr[19];

    // failsafes
    if(dst==(void*)0 || fmt==(void*)0) {
        return 0;
    }

    // main loop
    arg = 0;
    while(*fmt) {
        // argument access
        if(*fmt=='%') {
            fmt++;
            // literal %
            if(*fmt=='%') {
                goto put;
            }
            len=0;
            // size modifier
            while(*fmt>='0' && *fmt<='9') {
                len *= 10;
                len += *fmt-'0';
                fmt++;
            }
            // skip long modifier
            if(*fmt=='l') {
                fmt++;
            }
            // character
            if(*fmt=='c') {
                arg = __builtin_va_arg(args, int32_t);
                *dst++ = (char)arg;
                fmt++;
                continue;
            } else
            // decimal number
            if(*fmt=='d') {
                arg = __builtin_va_arg(args, int32_t);
                // check input
                sign=0;
                if((int32_t)arg<0) {
                    arg*=-1;
                    sign++;
                }
                if(arg>99999999999999999L) {
                    arg=99999999999999999L;
                }
                // convert to string
                i=18;
                tmpstr[i]=0;
                do {
                    tmpstr[--i]='0'+(arg%10);
                    arg/=10;
                } while(arg!=0 && i>0);
                if(sign) {
                    tmpstr[--i]='-';
                }
                // padding, only space
                if(len>0 && len<18) {
                    while(i>18-len) {
                        tmpstr[--i]=' ';
                    }
                }
                p=&tmpstr[i];
                goto copystring;
            } else
            // hex number
            if(*fmt=='x') {
                arg = __builtin_va_arg(args, int64_t);
                // convert to string
                i=16;
                tmpstr[i]=0;
                do {
                    char n=arg & 0xf;
                    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
                    tmpstr[--i]=n+(n>9?0x37:0x30);
                    arg>>=4;
                } while(arg!=0 && i>0);
                // padding, only leading zeros
                if(len>0 && len<=16) {
                    while(i>16-len) {
                        tmpstr[--i]='0';
                    }
                }
                p=&tmpstr[i];
                goto copystring;
            } else
            // string
            if(*fmt=='s') {
                p = __builtin_va_arg(args, char*);
copystring:     if(p==(void*)0) {
                    p="(null)";
                }
                while(*p) {
                    *dst++ = *p++;
                }
            }
        } else {
put:        *dst++ = *fmt;
        }
        fmt++;
    }
    *dst=0;
    __builtin_va_end(args);
    // number of bytes written
    return dst-orig;
}

/**
 * Variable length arguments
 */
uint32_t sprintf(char *dst, char* fmt, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    return vsprintf(dst,fmt,args);
}
