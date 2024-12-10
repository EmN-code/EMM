#include <unistd.h>
#include <time_units.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <architecture.h>

#define THREAD_STACKSIZE_MAIN (KERNEL_CONF_STACKSIZE_MAIN)
#define THREAD_PRIORITY_MAIN (KERNEL_CONF_PRIORITY_MAIN)

void *thread_handler(void *arg) {
    while (1) {
        printf("Thread %s is running with priority %d\n", (char *)arg, thread_get_priority(thread_getpid()));
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char thread_stack_1[THREAD_STACKSIZE_MAIN];
    char thread_stack_2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                          THREAD_PRIORITY_MAIN - 1,
                                          CREATE_STACKTEST,
                                          thread_handler,
                                          "Thread 1",
                                          "thread_1");
    
    kernel_pid_t thread_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                          THREAD_PRIORITY_MAIN - 2,
                                          CREATE_STACKTEST,
                                          thread_handler,
                                          "Thread 2",
                                          "thread_2");
    
    sleep(5);
    printf("Changing priorities...\n");
    
    // Changing priority
    thread_set_priority(thread_1, THREAD_PRIORITY_MAIN - 2);
    thread_set_priority(thread_2, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main thread can perform other tasks or sleep
        sleep(10);
    }

    return 0;
}