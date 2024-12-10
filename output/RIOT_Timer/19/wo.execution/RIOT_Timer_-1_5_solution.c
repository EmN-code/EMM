#include "xtimer.h"
#include "kernel_defines.h"
#include "stdbool.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "stdio.h"
#include "stdatomic.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "msg.h"

#define INTERVAL (1U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_pid;

void *led_blink_thread(void *arg) {
    (void)arg;
    gpio_init(GPIO_PIN(0, 2), GPIO_OUT);
    while (1) {
        gpio_toggle(GPIO_PIN(0, 2));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    printf("Starting timer and LED blinker...\n");
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, led_blink_thread, NULL, "led_blink");
    if (thread_pid < 0) {
        printf("Error: Unable to create the thread.\n");
        return 1;
    }
    return 0;
}
