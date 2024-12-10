#include <ztimer.h>
#include <msg.h>
#include <xtimer.h>
#include <stdio.h>

#define QUEUE_SIZE (8)
#define INTERVAL (1000000U)  // 1 second

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

static void _callback(void *arg) {
    (void)arg;
    msg_t msg;
    // Send a message to the mailbox every interval
    printf("Message sent to mbox\n");
    mbox_post(&mbox, &msg, 0);
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, QUEUE_SIZE);

    // Initialize mailbox
    mbox_init(&mbox, queue, QUEUE_SIZE);

    // Initialize timer event
    ztimer_t timer;
    ztimer_set(&ZTIMER_SEC, &timer, INTERVAL, _callback, NULL);

    puts("Message box example started\n");

    while (1) {
        msg_t msg;
        // Block until a message is received
        mbox_get(&mbox, &msg);
        printf("Message received\n");
    }

    return 0;
}