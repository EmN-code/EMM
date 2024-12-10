#include <ztimer.h>
#include <stdio.h>
#include <string.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Assuming ztimer is used for delay/alarm to mimic the RTC alarm feature
    // Set initial time to June 1, 2024 (this part is usually RTC specific)
    // Set an alarm for 5 seconds
    ztimer_sleep(ZTIMER_SEC, 5);
    
    // Register the callback
    alarm_callback(NULL);

    return 0;
}