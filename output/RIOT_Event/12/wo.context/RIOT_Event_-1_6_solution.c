#include "xtimer.h"
#include "thread.h"
#include "evtimer.h"
#include "msg.h"

#define INTERVAL_US (1000000U)  // 1 second in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];
static evtimer_t evtimer;
static evtimer_event_t event;

void my_callback(evtimer_event_t *event) {
    printf("Callback triggered\n");
}

int main(void) {
    puts("Event timer example\n");

    msg_init_queue(msg_queue, 8);
    evtimer_init(&evtimer, my_callback);
    evtimer_add(&evtimer, &event, INTERVAL_US);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)xtimer_dispatch, NULL,
                  "xtimer");

    return 0;
}