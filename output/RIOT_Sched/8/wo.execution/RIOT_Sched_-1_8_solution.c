/* The code demonstrates creating threads and changing their priority dynamically in RIOT OS. 
   We will include the necessary headers in the correct order, handle any compilation issues, 
   and ensure the implementation is properly structured.
*/

#include <ztimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <timex.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <time_units.h>

#define STACK_SIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char thread_stack_1[STACK_SIZE_MAIN];
char thread_stack_2[STACK_SIZE_MAIN];

void *thread_handler_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running with priority 1\n");
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }
    return NULL;
}

void *thread_handler_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running with priority 2\n");
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1 = thread_create(
        thread_stack_1, STACK_SIZE_MAIN, THREAD_PRIORITY_1, 0, thread_handler_1, NULL, "thread_1");

    kernel_pid_t thread_pid_2 = thread_create(
        thread_stack_2, STACK_SIZE_MAIN, THREAD_PRIORITY_2, 0, thread_handler_2, NULL, "thread_2");

    // Change priorities at runtime
    ztimer_sleep(ZTIMER_USEC, 5000000); // Sleep for 5 seconds
    printf("Changing priorities: Thread 1 to priority 2 and Thread 2 to priority 1\n");

    sched_change_priority(thread_get(thread_pid_1), THREAD_PRIORITY_2);
    sched_change_priority(thread_get(thread_pid_2), THREAD_PRIORITY_1);

    while (1) {
        // Continue main task
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }
    return 0;
}