
#include <esp_system.h>
#include <stddef.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define STACKSIZE 1024
#define PRIORITY 5

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_func1, NULL, "thread1");
    if (pid1 <= 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_func2, NULL, "thread2");
    if (pid2 <= 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change the priority of the first thread after some time
    xtimer_sleep(5);
    thread_set_priority(pid1, PRIORITY + 1);
    printf("Changed priority of thread 1\n");

    return 0;
}
