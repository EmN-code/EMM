
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "thread.h"

#define QUEUE_SIZE (8)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(1);  // Send a message every second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, SENDER_STACKSIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
