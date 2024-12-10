#include <ztimer/periodic.h>
#include <msg.h>
#include <stdio.h>
#include <event.h>
#include <evtimer.h>
#include <kernel_types.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
#define PERIOD              (1000000U)

static mbox_t message_box;
static msg_t message_queue[8];
static char stack[THREAD_STACKSIZE_MAIN];
static ztimer_periodic_t periodic_timer;

static void _send_message(void) {
    msg_t msg;
    msg.content.value = 42;  // Arbitrary value for the message
    mbox_send(&message_box, &msg);
    printf("Message sent: %ld\n", msg.content.value);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&message_box, message_queue, 8);

    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, PERIOD, _send_message);

    while (1) {
        ztimer_periodic_wait(&periodic_timer);
    }
    return 0;
}