
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Define the message structure
typedef struct {
    int type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

// Define the thread function prototypes
void *high_priority_thread(void *arg);
void *low_priority_thread(void *arg);

// Global message queues
msg_t high_priority_queue[16];
msg_t low_priority_queue[16];

int main(void)
{
    // Initialize the message queues
    msg_init_queue(high_priority_queue, 16);
    msg_init_queue(low_priority_queue, 16);

    // Create high priority thread
    char high_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(high_stack, sizeof(high_stack), THREAD_PRIORITY_MAIN - 1, 0, high_priority_thread, NULL, "high_priority");

    // Create low priority thread
    char low_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(low_stack, sizeof(low_stack), THREAD_PRIORITY_MAIN + 1, 0, low_priority_thread, NULL, "low_priority");

    // Main thread sends messages to both threads
    msg_t msg;
    msg.type = 1;
    msg.content.value = 123;

    // Send message to high priority thread
    msg_send_receive(&msg, &msg, thread_getpid_of(high_priority_thread));

    // Send message to low priority thread
    msg_send_receive(&msg, &msg, thread_getpid_of(low_priority_thread));

    return 0;
}

// High priority thread function
void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

// Low priority thread function
void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}
