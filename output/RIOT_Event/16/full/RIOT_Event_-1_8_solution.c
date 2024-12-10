#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "event/callback.h"
#include "periph/gpio.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "stddef.h"

#define INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    puts("Periodic event executed.");
}

int main(void) {
    msg_t msg_queue[8];

    thread_t *thread = thread_create((char *)msg_queue,
                                     sizeof(msg_queue),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     (thread_task_func_t)timer_callback,
                                     NULL,
                                     "periodic_thread");

    printf("RIOT Timer Example
");
    xtimer_sleep(1);

    while (1) {
        xtimer_periodic_wakeup(&(msg_queue[0].content.value), INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}