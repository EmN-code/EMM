
#include "xtimer.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static msg_t msg_queue[8];
static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the periodic event here
    }

    return 0;
}
