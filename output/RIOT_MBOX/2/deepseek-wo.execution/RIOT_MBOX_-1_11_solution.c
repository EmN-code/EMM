
#include "evtimer_msg.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "mbox.h"

#define STACK_SIZE 512
#define PRIORITY   THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received event!\n");
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.callback = event_handler;
    event.event.arg = NULL;
    event.msg.type = 0;
    event.msg.content.ptr = NULL;

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, event_handler, NULL, "event_handler");

    while (1) {
        evtimer_add_msg(&evtimer, &event, pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
