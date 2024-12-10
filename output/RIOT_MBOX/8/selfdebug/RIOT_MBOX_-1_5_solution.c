#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <evtimer.h>
#include <kernel.h>  // Ensure this file is within the include path.

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void _msg_callback(evtimer_t *evt, evtimer_event_t *event, void *arg) {
    msg_t msg;
    (void) evt;
    (void) event;
    (void) arg;
    printf("Message received\n");
    msg_receive(&msg);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);
    evtimer_event.msg.type = 0x1234;
    evtimer_event.event.offset = US_PER_SEC;
    
    evtimer_add_msg(&evtimer, &evtimer_event, thread_getpid());

    while (1) {
        xtimer_sleep(1);
        evtimer_add_msg(&evtimer, &evtimer_event, thread_getpid());
    }

    return 0;
}