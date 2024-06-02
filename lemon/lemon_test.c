#define LEMON_IMPLEMENTATION
#include "lemon.h"

int main() {
    lemon_byte_t heap[4096];

    lemon_init(heap, 4096);

    lemon_print();

    void *p32 = lemon_malloc(32);
    lemon_print();

    void *p64 = lemon_malloc(64);
    lemon_print();

    void *p128 = lemon_malloc(128);
    lemon_print();

    for(int i = 0; i < 5; ++i) {
        void *a = lemon_malloc(78);
        lemon_print();

        void *b = lemon_malloc(123);
        lemon_print();
        lemon_free(b);

        lemon_free(a);
    }

    void *p256 = lemon_malloc(256);
    lemon_print();

    void *p512 = lemon_malloc(512);
    lemon_print();

    lemon_free(p128);
    lemon_print();

    lemon_free(p512);
    lemon_print();

    lemon_free(p256);
    lemon_print();

    lemon_free(p32);
    lemon_print();

    lemon_free(p64);
    lemon_print();

    return 0;
}