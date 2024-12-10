#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <esp_system.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <mbox.h>
#include <msg.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sched.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <thread_arch.h>

// Declare thread information
char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Perform thread-specific tasks
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

// Main function
int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    // Create first thread
    thread1_pid = (kernel_pid_t) thread_func;

    // Create second thread
    thread2_pid = (kernel_pid_t) thread_func;

    // Change priority of the first thread
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 1);

    // Run main loop
    while (1) {
        // Perform main tasks or yield execution
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}