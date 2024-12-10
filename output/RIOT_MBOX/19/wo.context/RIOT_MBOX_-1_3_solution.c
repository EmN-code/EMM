#include <xtimer.h> // for US_PER_SEC and xtimer utility functions
#include <thread.h> // for thread-related functions
#include <stdio.h> // standard I/O operations
#include <msg.h> // for message box definitions

#define INTERVAL       (5U * US_PER_SEC) // 5 seconds interval
#define MSG_QUEUE_SIZE (8U) // Size of the message queue

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t my_pid;

void timer_callback(void *arg) {
    msg_t msg;
    msg_send(&msg, my_pid);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    my_pid = thread_get_active()->pid; // Get the current active thread ID

    xtimer_t my_timer;
    my_timer.callback = timer_callback;
    my_timer.arg = NULL;
    my_timer.target = 0;

    printf("Starting periodic message sending every %u microseconds...\n", INTERVAL);

    while (1) {
        xtimer_set_msg(&my_timer, INTERVAL, &msg_queue[0], my_pid);

        // Wait for the message (blocking)
        msg_t msg;
        msg_receive(&msg);

        printf("Message received!\n");
    }

    return 0;
}