#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph_cpu.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "MQTTClient.h"

#include <xtimer.h>  // Used for the timer to trigger the alarm.

void alarm_callback(void *arg) {
    (void)arg; // Unused variable
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize the RTC device
    rtc_init();

    // Set the initial date and time to June 1, 2024 at 00:00:00
    struct tm time = {0};
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;

    rtc_set_time(&time);

    puts("RTC set to June 1, 2024 00:00:00");

    // Set the alarm to trigger after 5 seconds
    xtimer_sleep(5);

    alarm_callback(NULL);

    return 0;
}