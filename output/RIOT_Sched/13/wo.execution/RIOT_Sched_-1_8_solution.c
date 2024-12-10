#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <thread.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

static void *thread_func(void *arg) {
    (void) arg;
    while(1) {
        printf("Running thread %d\n", thread_getpid());
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    ztimer_sleep(ZTIMER_SEC, 5);
    printf("Changing priority of threads\n");

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}