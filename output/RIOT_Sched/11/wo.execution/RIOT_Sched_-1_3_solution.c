#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5

char thread_stack_one[THREAD_STACKSIZE_DEFAULT];
char thread_stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    kernel_pid_t pid = *(kernel_pid_t *)arg;
    msg_t msg;
    msg_receive(&msg);
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    pid1 = thread_create(thread_stack_one, sizeof(thread_stack_one),
                         THREAD_PRIORITY_MAIN - 1, 0,
                         thread_handler, &pid1, "Thread 1");

    pid2 = thread_create(thread_stack_two, sizeof(thread_stack_two),
                         THREAD_PRIORITY_MAIN - 2, 0,
                         thread_handler, &pid2, "Thread 2");

    // Change priority of threads
    sched_set_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    sched_set_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    // Sleep for some time
    ztimer_sleep(ZTIMER_SEC, 1);

    return 0;
}