#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "event.h"

#define TIMER_INTERVAL_SEC (1U)

static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    puts("Timer callback fired!");
}

int main(void) {
    ztimer_set(&timer, ZTIMER_MSEC_BASE, TIMER_INTERVAL_SEC * MS_PER_SEC);
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_start(&timer);

    while (1) {
        thread_yield();
    }
    return 0;
}