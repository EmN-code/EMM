#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <xtimer.h>
#include <event/callback.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph_cpu.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    /* Initialize the RTC */
    rtc_init();

    /* Set the date and time to June 1, 2024, 00:00:00 */
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5; // months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(mktime(&time));

    /* Set an alarm for 5 seconds later */
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5; // add 5 seconds
    rtc_set_alarm(mktime(&alarm_time), alarm_callback, NULL);

    /* Use a thread to simulate running indefinitely */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
