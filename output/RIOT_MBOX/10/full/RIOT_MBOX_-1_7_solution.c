#include "netinet/in.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "timex.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mutex.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY   (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static msg_t queue[8];  // A power of two as required by msg_init_queue

void *msg_sender(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    msg.type = 1;  // Some message type identifier
    while (1) {
        xtimer_sleep(2);  // Send a message every 2 seconds
        msg_send(&msg, thread_getpid());  // Send to the same thread
    }
    return NULL;
}

int main(void) {
    printf("Message example
");

    // Initialize the message queue
    msg_init_queue(queue, 8);

    // Create a sender thread
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), PRIORITY, 0,
                                            msg_sender, NULL, "msg_sender");

    msg_t msg;
    while (1) {
        // Receive message
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }

    return 0;
}