#include "mbox.h"
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL  (1000000U) // 1 second in microseconds

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static mbox_t mbox;
static char _recv_stack[THREAD_STACKSIZE_MAIN];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (mbox_fetch(&mbox, &msg) < 1) {
            printf("No message received\n");
            continue;
        }
        printf("Received message with content: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    random_bytes_t seed;
    random_bytes(&seed, sizeof(seed));

    mbox_init(&mbox, msg_queue, 8);
    thread_create(_recv_stack, sizeof(_recv_stack), PRIORITY, 0, receiver, NULL, "receiver");

    msg_t msg;
    while (1) {
        seed.initial++;  // Some changing data
        msg.content.value = seed.initial;
        mbox_post(&mbox, &msg, KERNEL_MSG_BLOCK);
        printf("Sent message with content: %d\n", msg.content.value);
        xtimer_usleep(INTERVAL);
    }
    return 0;
}