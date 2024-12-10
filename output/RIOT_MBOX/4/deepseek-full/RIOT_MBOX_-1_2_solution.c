
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"
#include "mbox.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "timex.h"

#define QUEUE_SIZE 8
#define SEND_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 1;  // Example event
        mbox_put(&mbox, &msg);
        xtimer_sleep(SEND_INTERVAL);
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_init_mbox(&evtimer, &mbox);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)sender_thread,
                  NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)receiver_thread,
                  NULL, "receiver");

    return 0;
}
