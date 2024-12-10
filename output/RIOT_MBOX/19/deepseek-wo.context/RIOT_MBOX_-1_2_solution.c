
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include <stdio.h>

#define MSG_QUEUE_SIZE 8
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    msg_t received_msg;
    while (1) {
        mbox_get(&mbox, &received_msg, 1); // Use the correct function signature
        printf("Received message with value: %lu\n", (unsigned long)received_msg.content.value); // Correct format specifier
    }

    return 0;
}
