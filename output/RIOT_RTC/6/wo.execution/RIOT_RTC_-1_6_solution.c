#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>

#define TIMER_INTERVAL (5U * US_PER_SEC)

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = alarm_callback;
    timer.arg = NULL;

    tm now = {0};
    now.tm_year = 2024 - 1900; // Year since 1900
    now.tm_mon = 5; // June (0-based index)
    now.tm_mday = 1;
    now.tm_hour = 0;
    now.tm_min = 0;
    now.tm_sec = 0;

    // Set RTC to desired initial time
    rtc_set_time(&now);

    // Schedule an alarm to trigger after 5 seconds
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);

    // Wait for interrupt
    while(1) {
        // This is here to keep main thread alive.
    }

    return 0;
}