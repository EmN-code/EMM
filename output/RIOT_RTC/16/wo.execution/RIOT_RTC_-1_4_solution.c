#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"
#include "unistd.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "time_units.h"
#include "mma8x5x_params.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Argument is unused, silencing compiler warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_struct;
    time_struct.tm_year = 2024 - 1900; // Year since 1900
    time_struct.tm_mon = 6 - 1;        // Month 0-11
    time_struct.tm_mday = 1;          // Day of the month
    time_struct.tm_hour = 0;
    time_struct.tm_min = 0;
    time_struct.tm_sec = 0;

    // Initialize RTC with the specified time
    if (rtc_set_time(&time_struct) != 0) {
        printf("Error setting time.\n");
        return -1;
    }

    // Set an alarm for 5 seconds later
    xtimer_sleep(5); // wait for 5 seconds
    rtc_set_alarm(&time_struct, rtc_alarm_callback, NULL);

    while (1) {
        // Keeping the program running to wait for the alarm
        xtimer_sleep(1);
    }

    return 0;
}