
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MBOX_SIZE           (4)
static msg_t _mbox_queue[MBOX_SIZE];
static mbox_t _mbox;

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&_mbox, &msg);  // Corrected function call
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&_mbox, _mbox_queue, MBOX_SIZE);

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    msg_t msg;
    msg.content.value = 0;

    while (1) {
        xtimer_sleep(1);
        msg.content.value++;
        mbox_put(&_mbox, &msg);
    }

    return 0;
}
