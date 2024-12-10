#include <ztimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdbool.h>
#include <xtimer.h>
#include <time_units.h>
#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <errno.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <mutex.h>
#include <timex.h>

void alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;

    // Initialize RTC
    rtc_init();

    // Set date: June 1, 2024, 00:00
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1;        // tm_mon is months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Assuming rtc_set_time sets the time, although it's blacklisted
    // rtc_set_time(&time);

    // Set alarm 5 seconds after the current time
    time.tm_sec += 5;

    rtc_alarm_cb_t callback = alarm_cb;

    // Set the alarm
    rtc_set_alarm(&time, callback, NULL);

    // Simple loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}