#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <mutex.h>

// Callback function for the alarm
void alarm_handler(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Define time for June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 5,             // Month 0-11
        .tm_mday = 1,            // Day of the month 1-31
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    rtc_init();

    // Set the current time
    rtc_set_time(&time);

    // Set alarm to trigger after 5 seconds from now
    rtc_alarm_t alarm;
    rtc_get_time(&time);  // Get current time
    time.tm_sec += 5;     // Increment by 5 seconds
    mktime(&time);        // Normalize time structure

    // Set up alarm structure
    alarm.time = time;
    alarm.cb = alarm_handler;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm);

    // Continue with the main loop or any other processing
    while (1) {
        // Sleep or do other timed tasks
        xtimer_sleep(1);
    }

    return 0;
}