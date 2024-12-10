#include <ztimer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <event/periodic.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <periph/rtc.h>

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_DEFAULT (1024)

static char stack[THREAD_STACKSIZE_DEFAULT];

void *periodic_event_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Periodic event triggered\n");
        sleep(1); // Sleep for 1 second to simulate periodic event
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                      THREAD_PRIORITY, CREATE_STACKTEST,
                                      periodic_event_handler, NULL, "event_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread\n");
        return -1;
    }
    
    while (1) {
        // Main thread continues to do other tasks if needed
    }
    return 0;
}