#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_entry(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        /* receive message */
        msg_receive(&msg);
        /* handle message type */
        if (msg.type == 0x1234) {
            /* specific handling */
        }
    }
    return NULL;
}

int main(void)
{
    /* initialize message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* create a thread */
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_entry, NULL, "thread");

    /* sending events at intervals */
    while (1) {
        msg_t msg;
        msg.type = 0x1234;

        /* send message */
        msg_send(&msg, thread_getpid());

        /* sleep for a while */
        xtimer_sleep(1);
    }

    return 0;
}