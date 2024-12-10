#include <unistd.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arch/time_units.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>

void *thread_func(void *arg) {
    // Thread task
    while(1) {
        printf("Thread running with priority: %d\n", sched_active_thread->priority);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    // Create threads
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), 1, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), 2, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");
    
    // Change their priority
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);
    
    printf("Threads created with PIDs: %d, %d\n", pid1, pid2);
    return 0;
}
