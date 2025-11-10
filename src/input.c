#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "input.h"

static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && s[n-1] == '\n') s[n-1] = '\0';
}

void get_string_input(const char *prompt, char *buffer, int len) {
    if (!buffer || len <= 0) return;
    while (1) {
        if (prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }
        if (!fgets(buffer, len, stdin)) {
            clearerr(stdin);
            continue;
        }
        trim_newline(buffer);
        if (strlen(buffer) == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }
        return;
    }
}

int get_integer_input(const char *prompt) {
    char buf[128];
    char *endptr = NULL;
    long val;
    while (1) {
        if (prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }
        if (!fgets(buf, sizeof(buf), stdin)) {
            clearerr(stdin);
            continue;
        }
        // strip newline
        size_t n = strlen(buf);
        if (n > 0 && buf[n-1] == '\n') buf[n-1] = '\0';

        errno = 0;
        endptr = NULL;
        val = strtol(buf, &endptr, 10);
        if (errno != 0 || endptr == buf || *endptr != '\0') {
            printf("Invalid integer. Try again.\n");
            continue;
        }
        if (val < INT_MIN || val > INT_MAX) {
            printf("Integer out of range. Try again.\n");
            continue;
        }
        return (int)val;
    }
}
