#include <xtimer.h>
#include <msg.h>
#include <evtimer.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _event_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Event received with type: %u\n", msg.type);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.msg.sender_pid = thread_getpid();
    event.event.offset = 1000000;
    event.event.period = 1000000;

    msg_t msg;
    msg_send(&msg, thread_getpid());

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        _event_callback(NULL);
        xtimer_sleep(1);
    }

    return 0;
}