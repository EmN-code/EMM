
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SENDER_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1234;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, SENDER_STACK_SIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
