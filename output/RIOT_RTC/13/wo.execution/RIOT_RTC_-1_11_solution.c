#include <ztimer.h>
#include <rtc.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <stdio.h>

#define ALARM_SIGNAL_MSG 0x80

static msg_t _msg_queue[8];

void alarm_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void)
{
    msg_init_queue(_msg_queue, 8);

    printf("Setting up RTC...");

    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon  = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    if (rtc_set_time(&time) < 0) {
        printf("Error: Could not set RTC time\n");
        return -1;
    }

    ztimer_callback_t alarm_cb;
    alarm_cb.callback = alarm_callback;
    alarm_cb.arg = NULL;

    ztimer_set_timeout(RTC, &alarm_cb, ztimer_now(RTC) + 5U * US_PER_SEC);

    printf("Alarm set for 5 seconds later\n");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.content.value == ALARM_SIGNAL_MSG) {
            alarm_callback(NULL);
        }
    }
    return 0;
}