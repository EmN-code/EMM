#include "xtimer.h"
#include "stddef.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "architecture.h"
#include "arpa/inet.h"

void* thread_func(void *arg) {
    // Thread functionality here
    return NULL;
}

void* change_priority(void *arg) {
    kernel_pid_t main_thread_pid = thread_getpid();
    thread_t *main_thread = thread_get(main_thread_pid);

    // Changing priority at runtime
    sched_change_priority(main_thread, THREAD_PRIORITY_MAIN - 1);

    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    // Create two threads
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1, 
                  CREATE_STACKTEST, thread_func, NULL, "thread_func");

    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2, 
                  CREATE_STACKTEST, change_priority, NULL, "change_priority");

    // Demonstrating re-trying a network connection
    Network n;
    int rc;
    while (1) {
        rc = NetworkConnect(&n, "127.0.0.1", 1883);
        if (rc != 0) {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected successfully\n");
        break;
    }

    return 0;
}