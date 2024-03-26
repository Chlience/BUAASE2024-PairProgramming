#include <emscripten.h>
#include <stdio.h>
#include <string.h>

EMSCRIPTEN_KEEPALIVE
int bocchiShutUp (int flag, int seq[], int size) {
    int cnt[30];
    memset(cnt, 0, sizeof(cnt));
    int max = 0;
    int max_num = 0;
    for (int i = 0; i < size; ++ i) {
        if (seq[i] / 10 % 10 == flag) {
            ++ cnt[seq[i]];
            if (cnt[seq[i]] > max) {
                max = cnt[seq[i]];
                max_num = seq[i];
            }
        }
    }
    for (int i = 11; i <= 26; ++ i) {
        if (cnt[i] == max && i != max_num) {
            return 10;
        }
    }
    return max_num;
}


EMSCRIPTEN_KEEPALIVE
int getArr(int a[], int b) {
    return a[b];
}