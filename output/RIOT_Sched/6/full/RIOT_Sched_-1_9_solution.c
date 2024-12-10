#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdint.h"
#include "string.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "unistd.h"
#include "MQTTClient.h"

// Function that will be executed by each thread
void *thread_handler(void *arg) {
    while(1) {
        printf("Thread running: %s\n", (char*)arg);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1, 0,
                                         thread_handler, "Thread 1", "Thread 1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2, 0,
                                         thread_handler, "Thread 2", "Thread 2");

    // Change priority of thread1 to be higher
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN);

    // Change priority of thread2 to be lower
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN + 3);

    return 0;
}