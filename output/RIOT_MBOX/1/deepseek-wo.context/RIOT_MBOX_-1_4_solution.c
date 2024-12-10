
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    uint32_t counter = 0;
    while (1) {
        msg_t msg;
        msg.content.value = counter++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
