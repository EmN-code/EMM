#include <stdlib.h>
#include <msg.h>
#include <kernel.h>
#include <evtimer.h>
#include <xtimer.h>

#define INTERVAL       (1U * US_PER_SEC)  // 1 second
#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static evtimer_t event_timer;

void _event_handler(evtimer_t *etimer, void *arg) {
    (void)etimer;
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    evtimer_init(&event_timer, _event_handler, NULL);

    evtimer_set(&event_timer, INTERVAL);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
    }

    return 0;  // though this will likely never be reached
}