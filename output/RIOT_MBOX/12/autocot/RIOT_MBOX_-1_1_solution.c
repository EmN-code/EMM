#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t _msg_queue[MAIN_QUEUE_SIZE];

void sender_thread(void) {
    msg_t msg;
    int interval = 5000000U; // 5 seconds interval in microseconds
    unsigned i = 0;
    while (1) {
        i++;
        msg.content.value = i;
        printf("Sending message: %u\n", i);
        msg_send(&msg, sched_active_pid);
        xtimer_usleep(interval);
    }
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            0, sender_thread, NULL,
                                            "sender_thread");
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %u\n", msg.content.value);
    }
    return 0;
}