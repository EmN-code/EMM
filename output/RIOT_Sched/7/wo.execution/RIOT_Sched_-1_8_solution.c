#include <sys/socket.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <esp_system.h>
#include <stdbool.h>
#include <mutex.h>
#include <MQTTClient.h>   // Note: This typically isn't used in RIOT for ESP32, kept for requirement compliance
#include <sched.h>
#include "event/thread.h" // Using quotes assuming local includes
#include <paho_mqtt.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <timex.h>
#include <architecture.h>
#include <msg.h>
#include <xtimer.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <evtimer_msg.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char thread_stack_one[THREAD_STACKSIZE_MAIN];
static char thread_stack_two[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread_one;
static kernel_pid_t thread_two;

void *thread_func(void *arg) {
    while (1) {
        printf("Thread %ld is running\n", (intptr_t)arg);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting thread priority change example on ESP32 with RIOT");

    unsigned flags = 0;

    thread_one = thread_create(
            thread_stack_one, sizeof(thread_stack_one),
            THREAD_PRIORITY_MAIN + 1, flags,
            thread_func, (void *)1, "thread_one");

    thread_two = thread_create(
            thread_stack_two, sizeof(thread_stack_two),
            THREAD_PRIORITY_MAIN + 2, flags,
            thread_func, (void *)2, "thread_two");

    xtimer_sleep(2);

    // Change the priority of the threads
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 1);

    // Let threads run
    while (1) {
        puts("Main thread is running");
        xtimer_sleep(3);
    }

    return 0;
}