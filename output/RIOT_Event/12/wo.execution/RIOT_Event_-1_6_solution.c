#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "mutex.h"
#include "msg.h"

#define INTERVAL_SEC (1U) // Interval of 1 second

static msg_t _msg_queue[4];  // Message queue, must be power of two

void callback_function(void) {
    printf("Callback triggered every second\n");
}

void *thread_handler(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);  // Wait for the initial interval

    while(1) {
        msg_receive(&msg);  // Wait for an event
        callback_function();  // Call the callback function
        ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);  // Sleep for next interval
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, 4);  // Initialize message queue
    kernel_pid_t pid = thread_create((char[THREAD_STACKSIZE_DEFAULT]){},
                                     THREAD_STACKSIZE_DEFAULT,
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     thread_handler,
                                     NULL,
                                     "thread");
    
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
    }

    return 0;
}