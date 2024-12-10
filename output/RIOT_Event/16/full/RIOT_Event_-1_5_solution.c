#include "ztimer.h"
#include "stddef.h"
#include "stddef.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "timex.h"
#include "ztimer.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer/host.h"
#include <stdio.h>

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Callback function executed every second.\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_DEFAULT];
    (void) thread_handler;
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     thread_handler, NULL, "event callback");
    
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1) {
       // Main loop doing nothing, as the work is done in the thread
       ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}