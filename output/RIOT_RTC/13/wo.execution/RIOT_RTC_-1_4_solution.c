#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"

void alarm_handler(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the date and time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 6 - 1;        // Months since January [0-11]
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    time_t time_to_set = mktime(&timeinfo);
    rtc_set_time(&time_to_set);

    // Set the alarm to trigger 5 seconds after the current time
    evtimer_t alarm;
    evtimer_init_msg(&alarm, ztimer_clock_t_get_pid(ZTIMER_CLK), 5 * US_PER_SEC, mbox_get(), 0, alarm_handler);

    return 0;
}