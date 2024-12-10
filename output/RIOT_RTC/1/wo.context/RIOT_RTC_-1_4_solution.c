// This RIOT-OS code sets up an RTC alarm on an ESP32 using the specified RTC libraries.
// It initializes the date to June 1, 2024 and sets an alarm for 5 seconds later.

#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "periph/rtc.h"      // Required for RTC functions
#include "xtimer.h"          // Required for xtimer_sleep function
#include "arpa/inet.h"       
#include "MQTTClient.h"
#include "dht_params.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "string.h"
#include "time_units.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;   // tm_year is year since 1900
    time.tm_mon = 5;              // tm_mon range is 0-11
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm for 5 seconds from now
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Keep the main thread running to catch the alarm
    while (1) {
        xtimer_sleep(1);  // Prevent main thread from exiting
    }

    return 0;
}