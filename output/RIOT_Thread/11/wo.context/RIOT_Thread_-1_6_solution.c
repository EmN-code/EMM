#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char thread_stack[THREAD_STACKSIZE];
msg_t msg_queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    int cnt = 0;

    while (1) {
        // Prepare message
        msg.type = cnt;
        msg.content.value = cnt;
        // Send the message to the main thread
        msg_send(&msg, thread_getpid());
        printf("Sent msg %d\n", cnt);
        cnt++;
        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    // Launch a new thread
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    msg_t msg;
    while (1) {
        // Receive messages
        msg_receive(&msg);
        printf("Received msg %d\n", msg.content.value);
    }

    return 0;
}