#include <xtimer.h>
#include <stdio.h>
#include <stddef.h>
#include <msg.h>
#include <kernel_defines.h>
#include <kernel_types.h>

#define QUEUE_SIZE 8
#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    unsigned counter = 0;

    while (1) {
        msg.content.value = counter;
        mbox_post(&mbox, &msg, KERNEL_MSG_WAIT);
        printf("Sent message with value: %u\n", counter);
        counter++;
        xtimer_sleep(TIMER_INTERVAL);
    }
}

int main(void) {
    mbox_init(&mbox, queue, QUEUE_SIZE);
    printf("Mailbox initialized. Starting message sender...\n");

    // Create and start the sender thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    return 0;
}