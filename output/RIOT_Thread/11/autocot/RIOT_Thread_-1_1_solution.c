#include <kernel/rtimer.h>
#include <kernel/periph/timer.h>
#include <kernel/msg.h>
#include <kernel/irq.h>
#include <kernel/thread.h>
#include <kernel/xtimer.h>
#include <board.h>
#include <stdio.h>

// Define stack sizes for threads
#define THREAD_STACKSIZE_MAIN (KERNEL_CONF_STACKSIZE_MAIN)

// Declare message queue
msg_t msg_queue[16];

// Function to implement high priority thread
void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        msg_send_receive(&msg, &msg, sched_thread_getpid());
        printf("High Priority: Received message with value %ld\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

// Function to implement low priority thread
void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        printf("Low Priority: Sent message with value %ld\n", msg.content.value);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for the main thread
    msg_init_queue(msg_queue, 16);

    // Create high priority and low priority threads
    char high_priority_thread_stack[THREAD_STACKSIZE_MAIN];
    char low_priority_thread_stack[THREAD_STACKSIZE_MAIN];

    thread_create(high_priority_thread_stack, sizeof(high_priority_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  high_priority_thread, NULL, "high_priority_thread");

    thread_create(low_priority_thread_stack, sizeof(low_priority_thread_stack),
                  THREAD_PRIORITY_MAIN + 1, 0,
                  low_priority_thread, NULL, "low_priority_thread");

    // Idle loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main: Received message with value %ld\n", msg.content.value);
    }
    return 0;
}