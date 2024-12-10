#include <ztimer.h>
#include <timex.h>
#include <sched.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <thread.h>
#include <event/thread.h>
#include <xtimer.h>
#include <mbox.h>
#include <thread_config.h>

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    static char stack1[THREAD_STACKSIZE_MAIN];
    static char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN + 2);

    return 0;
}