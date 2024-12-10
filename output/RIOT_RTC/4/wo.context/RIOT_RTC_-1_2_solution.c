#include <ztimer.h>
#include <stdio.h>
#include <rtc.h>
#include <event/thread.h>
#include <event/callback.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    time.tm_year = 2024;
    time.tm_mon = 6;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    rtc_set_time(&time);
    printf("RTC time set to 2024-06-01 00:00:00\n");

    ztimer_periodic_callback_t alarm_event;
    event_periodic_callback_init(&alarm_event, ZTIMER_MSEC, NULL, alarm_callback);
    event_periodic_callback_set_interval(&alarm_event, 5000);  // 5 seconds
    event_periodic_callback_start(&alarm_event);

    // Allow the scheduler to trigger the alarm event
    ztimer_sleep(ZTIMER_SEC, 6);  // Sleep for slightly longer than the alarm interval

    return 0;
}