//
// Created by Gene on 2021/10/21.
//

#include "base/types.h"
#include "base/core_utils.h"

uint_8 core_strcpy(char *dst, char *src) {
    if (dst == null || src == null) {
        return 0;
    }

    int index = 0;

    while(true) {
        if (index > 64) {
            return index;
        }

        if (src[index] == '\0') {
            dst[index] = '\0';
            return index;
        }

        dst[index] = src[index];

        index++;
    }
}