
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];
static kernel_pid_t pid1, pid2;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void swap_priorities(void) {
    uint8_t prio1 = thread_get_priority(pid1);
    uint8_t prio2 = thread_get_priority(pid2);
    thread_change_priority(pid1, prio2);
    thread_change_priority(pid2, prio1);
    printf("Priorities swapped\n");
}

int main(void) {
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    while (1) {
        xtimer_sleep(5);
        swap_priorities();
    }

    return 0;
}
