#include <ztimer.h>
#include <xtimer.h>
#include <thread_config.h>
#include <time_units.h>
#include <timex.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <MQTTClient.h>

#define STACK_SIZE_THREAD_ONE 1024
#define STACK_SIZE_THREAD_TWO 1024
#define SLEEP_TIME_2 (2LU * US_PER_SEC)

static char stack_thread_one[STACK_SIZE_THREAD_ONE];
static char stack_thread_two[STACK_SIZE_THREAD_TWO];
kernel_pid_t pid_thread_one;
kernel_pid_t pid_thread_two;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        // Print a message
        printf("Thread One is running\n");
        sleep(2);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        // Print a message
        printf("Thread Two is running\n");
        sleep(2);
    }
    return NULL;
}

int main(void) {
    puts("Starting the program and creating threads\n");

    // Create Thread One
    pid_thread_one = thread_create(stack_thread_one, sizeof(stack_thread_one),
                                   THREAD_PRIORITY_MAIN, 0, thread_one,
                                   NULL, "Thread One");

    // Create Thread Two
    pid_thread_two = thread_create(stack_thread_two, sizeof(stack_thread_two),
                                   THREAD_PRIORITY_MAIN + 1, 0, thread_two,
                                   NULL, "Thread Two");

    sleep(5);

    // Change priority of Thread One
    sched_change_priority(thread_get(pid_thread_one), THREAD_PRIORITY_MAIN + 2);
    puts("Changed the priority of Thread One\n");

    while (1) {
        // Yield the main thread
        sleep(1);
    }

    return 0;
}