
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <thread.h>
#include <mbox.h>
#include <mma8x5x_regs.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_handler(void *arg)
{
    msg_t msg;
    msg_receive(&msg);
    printf("Received message: %d\n", (int)msg.content.value);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    kernel_pid_t pid = thread_getpid();

    for (int i = 0; i < 5; i++) {
        evtimer_add_msg(&evtimer, &event, pid);
        xtimer_sleep(1);
    }

    return 0;
}
