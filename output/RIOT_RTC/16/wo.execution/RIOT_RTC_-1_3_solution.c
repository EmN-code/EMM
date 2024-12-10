#include <ztimer.h>
#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <mma8x5x_regs.h>

#define ALARM_DURATION (5LU * US_PER_SEC)

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_struct = {0};
    time_struct.tm_year = 2024 - 1900; // tm_year is year since 1900
    time_struct.tm_mon = 6 - 1; // tm_mon is month since January (0-11)
    time_struct.tm_mday = 1;

    time_t rawtime;
    time(&rawtime); // Get the current time

    xtimer_usleep(ALARM_DURATION); // Wait for 5 seconds

    alarm_callback(NULL); // Call the alarm callback

    return 0;
}