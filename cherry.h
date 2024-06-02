#ifndef CHERRY_H
#define CHERRY_H

#include <math.h>
#include <time.h>

typedef struct CherryTimer {
    unsigned long long startTime;  
    unsigned long long endTime;

    double elapsedTime;  
} CherryTimer;

#define CHERRY_INLINE static inline

CHERRY_INLINE unsigned long long cherry_get_millis_internal();
CHERRY_INLINE void cherry_start_timer(CherryTimer* timer);
CHERRY_INLINE void cherry_stop_timer(CherryTimer* timer);

#define CHERRY_START_RECORD(LABEL) static CherryTimer timer ## LABEL; cherry_start_timer(&timer ## LABEL);
#define CHERRY_STOP_RECORD(LABEL) cherry_stop_timer(&timer ## LABEL);
#define CHERRY_GET_ELAPSED_TIME(LABEL) (timer ## LABEL).elapsedTime

#ifdef CHERRY_IMPLEMENTATION

unsigned long long cherry_get_millis_internal(){
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return (unsigned long long)(spec.tv_sec) * 1000 + (spec.tv_nsec / 1000000);
}

void cherry_start_timer(CherryTimer* timer) {
    timer->startTime = cherry_get_millis_internal();
}

void cherry_stop_timer(CherryTimer* timer) {
    timer->endTime = cherry_get_millis_internal();
    timer->elapsedTime = (double)(timer->endTime - timer->startTime); 
}

#endif

#endif