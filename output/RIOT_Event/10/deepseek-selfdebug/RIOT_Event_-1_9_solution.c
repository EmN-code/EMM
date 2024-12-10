
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, INTERVAL_MS * 1000);  // Convert milliseconds to microseconds

    return 0;
}
