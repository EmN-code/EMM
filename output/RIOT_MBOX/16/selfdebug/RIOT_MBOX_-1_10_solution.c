#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, 8);
    
    // Configuration for the thread
    static char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1, 0,
                                     thread_handler, NULL, "receiver_thread");

    // Sending messages to the mailbox
    for (int i = 0; i < 5; ++i) {
        msg_t msg;
        msg.content.value = i;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    
    return 0;
}