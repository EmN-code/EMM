
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <periph/i2c.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <unistd.h>
#include <time_units.h>
#include <stdbool.h>
#include <net/gcoap.h>
#include <msg.h>
#include <net/emcute.h>
#include <periph/rtc.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define MSG_QUEUE_SIZE (8)
#define ALARM_DELAY (5 * US_PER_SEC)  // 5 seconds

static msg_t msg_queue[MSG_QUEUE_SIZE];

void alarm_handler(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time_struct = {
        .tm_year = 2024 - 1900,  // years since 1900
        .tm_mon = 5,             // months since January (0-11)
        .tm_mday = 1,            // day of the month (1-31)
        .tm_hour = 0,            // hours since midnight (0-23)
        .tm_min = 0,             // minutes after the hour (0-59)
        .tm_sec = 0              // seconds after the minute (0-60)
    };
    rtc_set_time(&time_struct);

    // Set an alarm after 5 seconds
    ztimer_t alarm_timer;
    ztimer_set(ZTIMER_SEC, &alarm_timer, ALARM_DELAY);

    // Wait for the alarm to trigger
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_ALARM) {
            alarm_handler(NULL);
        }
    }

    return 0;
}
