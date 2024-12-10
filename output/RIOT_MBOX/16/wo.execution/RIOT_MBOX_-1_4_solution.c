#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_thread.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN      (1)
#define THREAD_CREATE_STACKTEST   0

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static mbox_t mbox;

void send_thread(void *arg) {
    (void)arg;
    msg_t message;
    message.content.value = 42; // example message
    while (1) {
        mbox_put(&mbox, &message);
        // Send message every second
        xtimer_sleep(1);
    }
}

int main(void) {
    printf("Starting mbox example\n");

    // Initialize mailbox
    mbox_init(&mbox, queue, 8);

    // Create sending thread
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN + 1,
                                      THREAD_CREATE_STACKTEST, send_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        // Wait and receive messages
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return 0;
}