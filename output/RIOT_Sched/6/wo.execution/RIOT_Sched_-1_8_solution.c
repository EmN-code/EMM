#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <sys/socket.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <esp_system.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN    (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

void *task(void *arg) {
    (void)arg;
    printf("Task thread running\n");
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, task, NULL, "thread1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, task, NULL, "thread2");

    printf("Main thread running\n");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        uint8_t prio1 = thread_get_priority((thread_t *)thread1_pid);
        uint8_t prio2 = thread_get_priority((thread_t *)thread2_pid);

        if (prio1 == THREAD_PRIORITY_MAIN - 1) {
            thread_set_priority((thread_t *)thread1_pid, THREAD_PRIORITY_MAIN - 2);
            thread_set_priority((thread_t *)thread2_pid, THREAD_PRIORITY_MAIN - 1);
        }
        else {
            thread_set_priority((thread_t *)thread1_pid, THREAD_PRIORITY_MAIN - 1);
            thread_set_priority((thread_t *)thread2_pid, THREAD_PRIORITY_MAIN - 2);
        }
    }
    return 0;
}