#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <evtimer.h>

#define MSG_QUEUE_SIZE  (8)
#define INTERVAL        (1LU * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

static void *event_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Event received: %d\n", msg.content.value);
    }
}

int main(void) {
    mbox_t mbox;
    evtimer_mbox_event_t event;
    evtimer_t evtimer;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init(&evtimer);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    unsigned int interval = INTERVAL;

    while (1) {
        xtimer_sleep(5);
        evtimer_add_mbox(&evtimer, &event, &mbox);
        printf("Event sent!\n");
    }

    return 0;
}