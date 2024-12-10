#include "ztimer.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "periph/gpio.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month range 0-11
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    rtc_set_time(&time);

    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;
    event.msg.content.ptr = NULL; // Setup event detail
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    ztimer_sleep(ZTIMER_SEC, 5); // wait for 5 seconds

    printf("Setting alarm for 5 seconds\n");
    // Assume the evtimer is set to trigger callback here

    msg_receive(&event.msg); // Wait until the message is received
    alarm_callback(NULL);

    return 0;
}