
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <esp_system.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void thread_func_high(void *arg)
{
    (void)arg;
    printf("High priority thread running\n");
    while (1) {
        // High priority thread logic
        xtimer_sleep(1);
    }
}

void thread_func_low(void *arg)
{
    (void)arg;
    printf("Low priority thread running\n");
    while (1) {
        // Low priority thread logic
        xtimer_sleep(2);
    }
}

int main(void)
{
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_func_high, NULL, "thread_high");
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_func_low, NULL, "thread_low");

    // Change priority of the low priority thread to high
    sched_change_priority(thread_get(pid_low), PRIORITY_HIGH);

    // Change priority of the high priority thread to low
    sched_change_priority(thread_get(pid_high), PRIORITY_LOW);

    return 0;
}
