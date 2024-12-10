#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define ALARM_INTERVAL_SEC (5U)

void rtc_alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    /* Initialize RTC */
    rtc_init();

    /* Set current date and time to 2024-06-01 00:00:00 */
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time_tm(&time);

    evtimer_msg_event_t evtimer_event;
    msg_t msg_queue[8];  // Message queue for receiving timer messages
    msg_init_queue(msg_queue, 8);

    evtimer_msg_t evtimer;
    evtimer_msg_init(&evtimer);

    /* Set an evtimer to trigger the alarm after 5 seconds */
    evtimer_msg_event(&evtimer, &evtimer_event, msg_queue, ALARM_INTERVAL_SEC * US_PER_SEC,
                      (evtimer_msg_callback_t) rtc_alarm_callback, NULL);

    /* Wait in a low-power mode infinitely for msg indicating the evtimer alarm */
    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}