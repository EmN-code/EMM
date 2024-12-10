// Import required headers in descending order by the first letter
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
// Import <MQTTClient.h> after resolving typedef conflicts
// Include event threading
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <thread.h>

void *periodic_task(void *arg) {
    while(1) {
        printf("Executing periodic task every second\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    char stack[THREAD_STACKSIZE_MAIN];

    // Create a new thread for the periodic task
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, periodic_task, NULL, "periodic");

    if (thread_pid <= 0) {
        printf("Error creating thread\n");
    }

    // Yield the thread to allow periodic execution
    thread_yield();

    return 0;
}