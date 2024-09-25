#define PLUM_IMPLEMENTATION
#include "plum.h"

int main() {
    PLUM_LOG(PLUM_INFO, "This is info message");
    PLUM_LOG(PLUM_WARNING, "This is a warning message");
    PLUM_LOG(PLUM_ERROR, "This is a error message");
    PLUM_LOG(PLUM_EXPERIMENTAL, "This is an experimental message");
    PLUM_LOG(PLUM_DEBUG, "This is a debug message");
    PLUM_LOG(-1, "This is an nil message");

    return 0;
}