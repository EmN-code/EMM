#include "thread_config.h"
#include "event/timeout.h"
#include "event/periodic.h"
#include <stdio.h>
#include "xtimer.h"
#include "ztimer.h"

static void alarm_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC time structure
    struct tm time_info;
    time_info.tm_year = 2024 - 1900; // Years since 1900
    time_info.tm_mon = 6 - 1;        // Months since January
    time_info.tm_mday = 1;           // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Set the current time
    ztimer_set(ZTIMER_EPOCH, &time_info);

    // Register an alarm for 5 seconds later
    uint32_t now = ztimer_now(ZTIMER_USEC);
    ztimer_set_msg(ZTIMER_USEC, now + 5 * US_PER_SEC, alarm_callback, NULL);

    while (1) {
        // Keep the main thread alive
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}