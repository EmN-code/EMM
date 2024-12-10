#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <kernel.h>

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_USEC       (500000)
#define MSG_TYPE_DATA       (0x1234)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void sender_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        msg.type = MSG_TYPE_DATA;
        printf("Sending message...\n");
        msg_send(&msg, sched_active_thread);  // Send to the active thread
        ztimer_sleep(ZTIMER_USEC, INTERVAL_USEC);
    }
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    char sender_thread_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_DATA) {
            printf("Received a message of type MSG_TYPE_DATA\n");
        }
    }

    return 0;
}