#include "xtimer.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "timex.h"
#include "unistd.h"
#include "time_units.h"

void *rtc_simulate_alarm(void *arg) {
    (void)arg;

    /* Wait 5 seconds, simulating time until the alarm */
    ztimer_sleep(ZTIMER_USEC, 5U * 1000000U);

    /* Alarm triggered message */
    printf("Alarm triggered!\n");
    return NULL;
}

int main(void) {
    printf("RTC Alarm Simulation\n");

    /* Create a thread to simulate the RTC alarm */
    char rtc_thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(rtc_thread_stack, sizeof(rtc_thread_stack), 5,
                  THREAD_CREATE_STACKTEST,
                  rtc_simulate_alarm, NULL, "rtc_alarm");

    /* Keep the main process alive */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}