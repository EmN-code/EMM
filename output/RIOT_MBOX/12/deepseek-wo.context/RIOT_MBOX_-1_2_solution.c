
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include <stdio.h>

#define MSG_QUEUE_SIZE (8)
#define INTERVAL_MS (1000000U)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 0);  // Fix: Use mbox_get with correct parameters
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);  // Fix: Use %lu for uint32_t
    }

    return 0;
}
