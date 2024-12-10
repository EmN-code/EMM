#include <ztimer.h>
#include <stdio.h>

#define TIME_DELAY 5  // 5 seconds alarm delay
#define RTC_FREQUENCY 1  // RTC frequency

/* Callback function to be executed when alarm triggers */
void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC with frequency
    if(rtc_init(RTC_FREQUENCY) == -1) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the current date to June 1, 2024
    struct tm time_info = {0};
    time_info.tm_year = 2024 - 1900;  // year since 1900
    time_info.tm_mon = 6 - 1;         // month index starting from 0
    time_info.tm_mday = 1;            // day of the month

    rtc_set_time(tm_mktime(&time_info));
    printf("Date set to June 1, 2024\n");

    // Set an alarm to trigger after 5 seconds using ztimer
    ztimer_t timer;
    ztimer_set(evtimer_msg, &timer, TIME_DELAY * ZTIMER_SEC, alarm_callback, NULL);

    // The system should continue running and will automatically
    // print "Alarm triggered!" after 5 seconds as the callback is invoked
    return 0;
}