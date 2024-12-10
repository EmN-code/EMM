
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "timer.h"
#include "periph/timer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MBOX_SIZE           (16)
static msg_t _mbox_queue[MBOX_SIZE];
static mbox_t _mbox;

#define TIMER_DEV           TIMER_DEV(0)
#define TIMER_SPEED         (1000000U)
#define TIMER_INTERVAL      (1000000U)

static void _timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    mbox_put(&_mbox, &msg);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&_mbox, _mbox_queue, MBOX_SIZE);

    timer_init(TIMER_DEV, TIMER_SPEED, _timer_cb, NULL);
    timer_set_periodic(TIMER_DEV, 0, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        mbox_get(&_mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
