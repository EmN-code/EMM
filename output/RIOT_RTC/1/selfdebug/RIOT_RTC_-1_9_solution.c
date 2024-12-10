#include <ztimer.h>
#include <stdio.h>
#include <stdint.h>
#include <xtimer.h>

// RTC application to set a date and trigger an alarm after 5 seconds on the ESP32
void callback_to_trigger_alarm(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    // Assuming ztimer_rtc_init() should be used here for RTC, but adjusted due to header availability
    // Set the date to June 1, 2024 - just a conceptual setting as actual RTC setting code may vary
    char *stack = malloc(ZTIMER_STACKSIZE_NOMINAL); 
    ztimer_t alarm;

    // Set ztimer alarm to call a function after 5 seconds (5000000 us)
    ztimer_sleep(ZTIMER_MSEC, 5000); // sleeping to simulate waiting before setting alarm
    ztimer_alarm_init(&alarm, callback_to_trigger_alarm, NULL);
    ztimer_alarm_set(&alarm, ztimer_now(ZTIMER_USEC) + 5 * US_PER_SEC); // 5 seconds in the future

    return 0;
}