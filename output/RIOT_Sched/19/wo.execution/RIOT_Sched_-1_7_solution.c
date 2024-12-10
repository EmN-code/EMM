
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "unistd.h"


#define THREAD_STACKSIZE (1024)
#define THREAD_PRIORITY_MAIN (5)
#define THREAD_PRIORITY_SECONDARY (6)

char stack_second_thread[THREAD_STACKSIZE];
kernel_pid_t main_thread_pid;
kernel_pid_t second_thread_pid;

void *second_thread_function(void *arg) {
    (void)arg;
    printf("Secondary thread is running\n");
    return NULL;
}

int main(void) {
    printf("Main thread is starting\n");

    main_thread_pid = thread_create(stack_second_thread,
                                    THREAD_STACKSIZE,
                                    THREAD_PRIORITY_MAIN,
                                    0,
                                    second_thread_function,
                                    NULL,
                                    "second_thread");

    ztimer_sleep(ZTIMER_MSEC, 1000);

    second_thread_pid = thread_create(stack_second_thread,
                                      THREAD_STACKSIZE,
                                      THREAD_PRIORITY_SECONDARY,
                                      0,
                                      second_thread_function,
                                      NULL,
                                      "secondary_thread");

    ztimer_sleep(ZTIMER_MSEC, 1000);

    printf("Changing priority of secondary thread\n");

    sched_change_priority(thread_get(second_thread_pid), THREAD_PRIORITY_MAIN);

    while(1) {
        thread_yield();
    }

    return 0;
}
