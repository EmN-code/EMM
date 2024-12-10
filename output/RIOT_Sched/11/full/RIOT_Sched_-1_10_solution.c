#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "modem/modem.h"

void *thread_func1(void *arg) {
    while (1) {
        // Thread 1 work
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        // Thread 2 work
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, CREATE_STACK_TEST, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, CREATE_STACK_TEST, thread_func2, NULL, "thread2");

    // Change priority runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main thread work
    }

    return 0;
}