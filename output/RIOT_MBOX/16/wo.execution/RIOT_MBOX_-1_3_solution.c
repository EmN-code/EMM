#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "ztimer.h"
#include "time_units.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY  (THREAD_PRIORITY_MAIN + 1)

static char stack[STACKSIZE];
static mbox_t mbox;
static msg_t mbox_queue[8];
static kernel_pid_t msg_thread_pid;

void *msg_thread(void *arg){ 
    (void)arg; 
    msg_t msg;
    while (1) {
        if (mbox_try_get(&mbox, &msg) == 1) {
            printf("Received a message!\n");
        }
        usleep(1000000); // Sleep for 1 second
    }
    return NULL; 
}

int main(void)
{
    puts("RIOT application - Mailbox example");

    // Initialize the message box
    mbox_init(&mbox, mbox_queue, 8);

    // Create a thread to process messages
    msg_thread_pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST,
                                   msg_thread, NULL, "msg_thread");

    msg_t msg;
    while (1) {
        // Send a message to the mailbox every 2 seconds
        msg.content.value = 123;
        mbox_try_put(&mbox, &msg);
        printf("Sent a message!\n");
        usleep(2000000); // Sleep for 2 seconds
    }

    return 0;
}