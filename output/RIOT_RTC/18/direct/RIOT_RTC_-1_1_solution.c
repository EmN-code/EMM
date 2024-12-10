#include <ztimer/ztimer.h>
#include <stdio.h>
#include <time.h>
#include <xtimer.h>

#define ALARM_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

void *alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
    return NULL;
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 5; // Month, where 0 = January
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set the current time
    time_t now = mktime(&timeinfo);

    // Delay execution to simulate future time set, in a real case it would use RTC hardware
    xtimer_usleep(1000000); 

    printf("Current time set to: %s", asctime(&timeinfo));

    // Set an alarm to go off 5 seconds from now
    ztimer_sleep(ZTIMER_USEC, ALARM_INTERVAL);

    alarm_handler(NULL);

    return 0;
}
