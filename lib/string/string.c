#include "string.h"

int strlen(const char *s) {
    const char *ptr = s;
    while (*ptr != '\0') ptr++;
    return ptr - s;
}

void strcpy(char *dest, const char *src) {
    while(*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}

void strcat(char *dest, const char *src) {
    while(*dest != '\0') dest++;
    while(*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}