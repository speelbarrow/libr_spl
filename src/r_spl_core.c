#include <r_spl_core.h>

bool * R_SPL_DEBUG;
bool * const r_spl_debug() {
    if (R_SPL_DEBUG == NULL) {
        R_SPL_DEBUG = malloc(sizeof(bool));
        (*R_SPL_DEBUG) = false;
    }

    return R_SPL_DEBUG;
}

