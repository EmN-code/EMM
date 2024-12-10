
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[8];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(2);  // Send a message every 2 seconds
        mbox_put(&mbox, &msg);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, 8);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);  // Blocking call to get a message from the mbox
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
