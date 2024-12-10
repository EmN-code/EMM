#include <ztimer.h>
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <MQTTClient.h>
#include <stdbool.h>
#include <stdbool.h>
#include <mqtt.h>
#include <net/sock/util.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <timex.h>
#include <msg.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <ztimer.h>
#include <event/thread.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mbox.h>
#include <esp_system.h>
#include <ztimer.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_mbox.h>
#include <net/sock/udp.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

static char thread_stack_one[THREAD_STACKSIZE_MAIN];
static char thread_stack_two[THREAD_STACKSIZE_MAIN];
kernel_pid_t pid_one, pid_two;

void *thread_func_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is running at priority %d\n", THREAD_PRIORITY_MAIN);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is running at priority %d\n", THREAD_PRIORITY_MAIN);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    pid_one = thread_create(
        thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN,
        0, thread_func_one, NULL, "thread_one");

    pid_two = thread_create(
        thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_MAIN + 1,
        0, thread_func_two, NULL, "thread_two");

    printf("Threads created with PIDs %d and %d\n", pid_one, pid_two);

    while (1) {
        if (kernel_pid_t thread = thread_get(pid_one)) {
            sched_change_priority(thread, THREAD_PRIORITY_MAIN + 2);
            printf("Changed priority of Thread One to %d\n", THREAD_PRIORITY_MAIN + 2);
        }

        if (kernel_pid_t thread = thread_get(pid_two)) {
            sched_change_priority(thread, THREAD_PRIORITY_MAIN);
            printf("Changed priority of Thread Two to %d\n", THREAD_PRIORITY_MAIN);
        }

        xtimer_sleep(5);
    }
    return 0;
}