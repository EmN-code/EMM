
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACK_SIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 10

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_mbox_t evtimer_mbox;

void event_handler(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Event received: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    evtimer_mbox_init(&evtimer_mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, event_handler, NULL, "event_handler");

    msg_t msg;
    msg.content.value = 1234;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000);
    xtimer_ticks32_t now = xtimer_now();
    xtimer_ticks32_t target = now + interval;

    evtimer_mbox_add_msg(&evtimer_mbox, &msg, target);

    return 0;
}
