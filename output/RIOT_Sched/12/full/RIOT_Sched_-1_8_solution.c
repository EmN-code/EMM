#include <architecture.h>
#include <esp_system.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <sched.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <thread.h>
#include <thread_config.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>


#define STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define STACKSIZE_HELPER THREAD_STACKSIZE_DEFAULT

static char main_stack[STACKSIZE_MAIN];
static char helper_stack[STACKSIZE_HELPER];

void *main_thread_func(void *arg) {
    (void)arg;
    kernel_pid_t main_thread = thread_getpid();
    while (1) {
        printf("Main thread running with priority: %u\n", thread_get_priority(thread_get(main_thread)));
        usleep(500000);
    }
    return NULL;
}

void *helper_thread_func(void *arg) {
    (void)arg;
    kernel_pid_t helper_thread = thread_getpid();
    while (1) {
        printf("Helper thread running with priority: %u\n", thread_get_priority(thread_get(helper_thread)));
        usleep(500000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t main_thread_pid = thread_create(
        main_stack, sizeof(main_stack),
        THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
        main_thread_func, NULL, "main");

    kernel_pid_t helper_thread_pid = thread_create(
        helper_stack, sizeof(helper_stack),
        THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
        helper_thread_func, NULL, "helper");

    usleep(2000000);
    printf("Changing priority of helper thread\n");
    sched_change_priority(thread_get(helper_thread_pid), THREAD_PRIORITY_MAIN - 1);

    usleep(10000000);
    printf("End of main\n");
    return 0;
}