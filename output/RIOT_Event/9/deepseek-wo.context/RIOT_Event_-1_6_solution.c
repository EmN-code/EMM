
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Event periodic callback triggered!\n");
    xtimer_set(&timer, TIMER_INTERVAL);
}

static xtimer_t timer;

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}
