#ifndef UTILS
#define UTILS

#ifndef WIN32
    #include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))


#endif // UTILS

