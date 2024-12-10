#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static char mbox_queue[64];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive messages
        mbox_get(&mbox, &msg);
        printf("Received a message in thread: %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, sizeof(mbox_queue));

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "thread_handler");

    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        msg.content.value = random_uint32() % 100;
        printf("Sending message: %d\n", (int)msg.content.value);
        mbox_put(&mbox, &msg);
        xtimer_sleep(2);
    }

    return 0;
}