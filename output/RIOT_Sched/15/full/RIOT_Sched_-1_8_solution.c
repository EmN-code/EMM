#include "esp_wifi.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <thread_arch.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN    (2 * THREAD_STACKSIZE_DEFAULT)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    while (1) {
        printf("Thread %d running\n", *((int *)arg));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;
    int thread1_id = 1;
    int thread2_id = 2;

    printf("Starting threads...\n");

    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, &thread1_id, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, &thread2_id, "thread2");

    printf("Changing priorities...\n");

    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}