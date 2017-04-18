#ifndef _ARCH_UM_TERMINAL_H
#define _ARCH_UM_TERMINAL_H

//#define DISABLE_COLOR
#ifndef DISABLE_COLOR
#define c_normal         "\x1b[0m"
#define c_bold           "\x1b[1m"
#define c_red            "\x1b[31m"
#define c_green          "\x1b[32m"
#define c_yellow         "\x1b[33m"
#define c_blue           "\x1b[34m"
#define c_purple         "\x1b[35m"
#define c_cyan           "\x1b[36m"
#else
#define c_normal         ""
#define c_bold           ""
#define c_red            ""
#define c_green          ""
#define c_yellow         ""
#define c_blue           ""
#define c_purple         ""
#define c_cyan           ""
#endif



#endif
