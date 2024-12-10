
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define HIGH_PRIO_THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define LOW_PRIO_THREAD_STACKSIZE     (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t high_prio_pid;
kernel_pid_t low_prio_pid;

void *high_prio_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *low_prio_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.ptr = "Hello from low priority thread!";
    msg.type = 0;

    while (1) {
        msg_send(&msg, high_prio_pid);
        printf("Low priority thread sent message\n");
        thread_sleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main(void)
{
    puts("RIOT inter-thread communication example");

    // Create high priority thread
    high_prio_pid = thread_create(
        thread_stack, HIGH_PRIO_THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        high_prio_thread, NULL, "high_prio_thread");

    // Create low priority thread
    low_prio_pid = thread_create(
        thread_stack, LOW_PRIO_THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        low_prio_thread, NULL, "low_prio_thread");

    // Main thread will not exit
    while (1) {
        thread_sleep(1000000); // Sleep for 1 second
    }

    return 0;
}
