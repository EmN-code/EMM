#include <xtimer.h> // For timing functions
#include <thread.h> // For thread related functions
#include <stdio.h> // For input/output functions like printf
#include <msg.h> // For message passing mechanisms

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

char stack[THREAD_STACKSIZE_MAIN];

static void *thread_func(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);  // Wait for a message to arrive
        printf("Received message: %d\n", msg.content.value);
        xtimer_sleep(1);  // Use ztimer_sleep if xtimer_sleep is implicitly declared
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            thread_func, NULL,
                                            "msg_thread");

    msg_t msg;

    while (1) {
        msg.content.value = 42;
        printf("Sending message: %d\n", msg.content.value);
        msg_send(&msg, thread_pid);  // Send message to the thread
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}