#ifndef __MACRO_H__
#define __MACRO_H__


#define R(x) (random() % (x))

#define MIN(_a,_b) ((_a) > (_b) ? (_b) : (_a))
#define MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))

#define TODO do {                          \
    SAYL(cPUR "TODO" cRST                  \
        " : implement %s() in %s:%u",      \
        __FUNCTION__, __FILE__, __LINE__); \
    exit(0);                               \
} while (0);

#define DEPRECATED do {                          \
    SAYL(cREDp "DEPRECATED" cRST                 \
        " : %s() have been deprecated in %s:%u", \
        __FUNCTION__, __FILE__, __LINE__);       \
    exit(-1);                                    \
} while(0);

#endif