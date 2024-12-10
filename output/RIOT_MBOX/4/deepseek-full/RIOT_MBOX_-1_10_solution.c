
#include "evtimer_msg.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "ztimer.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "xtimer.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void event_handler(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    msg_t msg;
    kernel_pid_t pid;

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create a thread to handle events
    pid = thread_create(NULL, 128, THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    // Send messages at intervals
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, pid);
        xtimer_sleep(1);
    }

    return 0;
}
