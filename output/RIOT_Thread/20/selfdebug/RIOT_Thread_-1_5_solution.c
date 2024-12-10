#include <xtimer.h> // For using xtimer_sleep
#include <thread.h>  // For threads
#include <msg.h>     // For msg_t related operations

#define STACK_SIZE_THREAD_A (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD_B (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_THREAD_A (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_B (THREAD_PRIORITY_MAIN - 2)

char stack_thread_a[STACK_SIZE_THREAD_A];
char stack_thread_b[STACK_SIZE_THREAD_B];

static msg_t msg_queue[8];

void *thread_a(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message value

    while (1) {
        // Send the message to thread B
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);  // Sleep for a specified time
    }
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    while (1) {
        // Wait for a message
        msg_receive(&msg);

        // Do something with the received message
        printf("Received message with value: %" PRIu32 " \n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread_a = thread_create(stack_thread_a, sizeof(stack_thread_a),
                                           PRIORITY_THREAD_A, THREAD_CREATE_STACKTEST,
                                           thread_a, NULL, "thread_a");

    kernel_pid_t pid_thread_b = thread_create(stack_thread_b, sizeof(stack_thread_b),
                                           PRIORITY_THREAD_B, THREAD_CREATE_STACKTEST,
                                           thread_b, NULL, "thread_b");

    msg_t main_msg;
    (void)pid_thread_a;
    (void)pid_thread_b;
    // Main thread loop, if necessary
    while (1) {
        msg_receive(&main_msg);
    }

    return 0;
}