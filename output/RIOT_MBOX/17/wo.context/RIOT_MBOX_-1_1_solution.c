#include <ztimer.h>  // For timer functionality
#include <xtimer.h>  // For providing high-level timer functions
#include <stdbool.h>
#include <stdio.h>    // For printf for debugging purposes

#define EVENT_PERIOD (1U * US_PER_SEC)  // Time period for events

static msg_t msg_queue[8];  // Define a message queue with a length of 8

static void _msg_send_timely(void)
{
    msg_t msg;
    msg.type = 0x8000;
    int count = 0;

    while (1) {
        printf("Sending message %d\n", count);
        msg.content.value = count++;  // Increment the message content value
        msg_send(&msg, thread_getpid());  // Send message to the current thread
        xtimer_sleep(EVENT_PERIOD / US_PER_SEC);  // Sleep for EVENT_PERIOD seconds
    }
}

int main(void)
{
    puts("Starting message box functionality...");
    msg_init_queue(msg_queue, 8);  // Initialize the message queue

    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, _msg_send_timely, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Block until a message is received
        printf("Received message with content: %d\n", msg.content.value);
    }

    return 0;
}
