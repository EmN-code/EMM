#include "thread.h"
#include "msg.h"

// Function prototypes
void *low_priority_thread(void *arg);
void *high_priority_thread(void *arg);

// Message queue setup
msg_t msg_q_low[8];  // Queue for low priority thread
msg_t msg_q_high[8]; // Queue for high priority thread

// Thread stack size
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

// Thread definitions
kernel_pid_t low_pid;
kernel_pid_t high_pid;
char stack_low[STACKSIZE];
char stack_high[STACKSIZE];

int main(void) {
    // Initialize message queues
    msg_init_queue(msg_q_low, sizeof(msg_q_low)/sizeof(msg_t));
    msg_init_queue(msg_q_high, sizeof(msg_q_high)/sizeof(msg_t));

    // Create low priority thread
    low_pid = thread_create(stack_low, STACKSIZE, THREAD_PRIORITY_MAIN + 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    // Create high priority thread
    high_pid = thread_create(stack_high, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    return 0;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Wait and receive a message
        msg_receive(&msg);
        printf("Low priority thread received a message\n");

        // Process received message
        // ...
    }

    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Compose a message
        msg.type = 0x1234;

        // Send message to low priority thread
        msg_send(&msg, low_pid);
        printf("High priority thread sent a message\n");

        // Sleep or wait some time
        xtimer_sleep(1);
    }

    return NULL;
}