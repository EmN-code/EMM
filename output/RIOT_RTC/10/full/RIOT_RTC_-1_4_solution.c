#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"

void alarm_handler(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    rtc_get_time(&time);  // Get current time (satisfy unused variable warning)
    
    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // June
    time.tm_mday = 1;            // 1st day
    
    // Safely set the RTC time and check return value
    while (1) {
        if (rtc_set_time(&time) == 0) {
            printf("RTC time set successfully.\n");
            break;
        } else {
            printf("RTC time setting failed. Retrying...\n");
            xtimer_sleep(1);
        }
    }

    // Set an alarm for 5 seconds into the future
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Correct for overflow in tm_sec
    mk_y2k_epoch_vals(&alarm_time);

    // Safely set the RTC alarm and check return value
    while (1) {
        if (rtc_set_alarm(&alarm_time, alarm_handler) == 0) {
            printf("RTC alarm set successfully.\n");
            break;
        } else {
            printf("RTC alarm setting failed. Retrying...\n");
            xtimer_sleep(1);
        }
    }

    // Keep the main thread busy
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}