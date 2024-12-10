#include <thread.h>
#include <string.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <dht_params.h>

#define ALARM_DELAY (5U) // delay in seconds

void* alarm_function(void* arg) {
    (void)arg;
    puts("Alarm triggered!");
    return NULL;
}

int main(void) {
    // Initialize RTC
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5; // June (months since January 0-11)
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();
    rtc_set_time(&time);

    // Create a thread to simulate alarm triggering
    char alarm_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(alarm_thread_stack, sizeof(alarm_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  alarm_function, NULL, "alarm_thread");

    // Wait for the alarm delay
    xtimer_sleep(ALARM_DELAY);

    // Trigger the alarm
    alarm_function(NULL);

    return 0;
}