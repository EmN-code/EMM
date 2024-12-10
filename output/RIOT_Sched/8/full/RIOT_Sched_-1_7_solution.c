#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sched.h>
#include <msg.h>
#include <mutex.h>
#include <thread.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mqttclient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <irq.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <thread_config.h>
#include <sys/socket.h>
#include <architecture.h>
#include <timex.h>
#include <time_units.h>

#define THREAD_STACKSIZE_DEFAULT  (1024U)
#define THREAD_PRIORITY_MAIN      (THREAD_PRIORITY_LEVELS-1)
#define THREAD_PRIORITY_SECOND    (THREAD_PRIORITY_LEVELS-2)

void *thread_func(void *arg) {
    (void)arg; // suppress unused parameter warning
    while (1) {
        // Main functionality of the thread
        printf("Thread is running with priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid;

    printf("Main thread started\n");

    // Create a new thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_SECOND, THREAD_CREATE_STACKTEST, thread_func, NULL, "new_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return -1;
    }

    // Simulate some processing in the main thread
    xtimer_sleep(5);

    // Change the priority of the created thread
    sched_change_priority(thread_get(thread_pid), THREAD_PRIORITY_MAIN);

    // Continue processing in the main thread
    while (1) {
        // Just print out messages indicating the main thread is running
        printf("Main thread running\n");
        xtimer_sleep(1);
    }
    return 0;
}
