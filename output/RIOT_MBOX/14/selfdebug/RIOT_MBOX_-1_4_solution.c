#include <ztimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MBOX_SIZE (8)

static char stack[THREAD_STACKSIZE_DEFAULT];
static mbox_t mbox;
static msg_t queue[MBOX_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        if (mbox_try_put(&mbox, &msg) == 1) {
            printf("Sent: %d\n", msg.content.value);
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg) == 1) {
            printf("Received: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("Message box example\n");

    mbox_init(&mbox, queue, MBOX_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, msg_sender, NULL, "msg_sender");

    msg_receiver(NULL);
    return 0;
}