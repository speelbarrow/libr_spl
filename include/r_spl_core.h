#ifndef R_SPL_CORE_H
#define R_SPL_CORE_H

#define R_SPL_Pragma(...) _Pragma(#__VA_ARGS__)
#ifdef __clang__
#define R_SPL_DIAGNOSTIC_OPEN(GROUP) \
    _Pragma("clang diagnostic push") \
    R_SPL_Pragma(clang diagnostic ignored #GROUP)
#define R_SPL_DIAGNOSTIC_CLOSE() \
    _Pragma("clang diagnostic pop")
#elif __GNUC__
#define R_SPL_DIAGNOSTIC_OPEN(GROUP) \
    _Pragma("GCC diagnostic push") \
    R_SPL_Pragma(GCC diagnostic ignored #GROUP)
#define R_SPL_DIAGNOSTIC_CLOSE() \
    _Pragma("GCC diagnostic pop")
#endif

#include <stdlib.h>
#include <stdbool.h>

bool * const r_spl_debug();

#define R_SPL_REPORT "%s: %s returned %d\n"
#define R_SPL_SETUP() \
if (argc == 2 && strcmp(argv[1], "-d") == 0) { \
    *(r_spl_debug()) = true; \
    printf("DBG ON\n"); \
} else if (argc > 1) { \
    printf("Usage: %s [-d]\n", argv[0]); \
    exit(1); \
}

#define CALL_AND_REPORT(EXPECT, F, ...) { \
    const int v = F(__VA_ARGS__); \
    char *prefix = "DBG"; \
    bool ret = false; \
    int out = 1; \
    R_SPL_DIAGNOSTIC_OPEN(-Wpointer-integer-compare) \
    if (sizeof(EXPECT) == sizeof(int) && EXPECT != v) { \
        R_SPL_DIAGNOSTIC_CLOSE() \
        prefix = "ERR"; \
        ret = true; \
        out = 2; \
    } \
    if (*(r_spl_debug()) == true || strcmp(prefix, "DBG") != 0) { \
        /* \
         * -6 because of the three format specifiers in `R_SPL_REPORT` \
         * +12 because the maximum length of an integer is 10 digits, plus the \
         * sign, plus the null terminator \
         */ \
        char *msg = malloc(sizeof(char) * (strlen(R_SPL_REPORT) - 4 + strlen(prefix) + strlen(#F) + 12)); \
        sprintf(msg, R_SPL_REPORT, prefix, #F, v); \
        write(out, msg, strlen(msg)); \
        free(msg); \
    } \
    if (ret) { \
        exit(1); \
    } \
}

#define R_SPL_TEARDOWN() { \
    if (socket != NULL) \
        r_socket_free(socket); \
    free(r_spl_debug()); \
}

#endif // R_SPL_CORE_H
