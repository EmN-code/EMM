
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
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
#include <sys/socket.h>
#include <stddef.h>
#include <ztimer.h>

#define MSG_TYPE_ALARM (0x1234)

static msg_t msg_queue[8];

void alarm_handler(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    rtc_init();

    struct tm time_struct = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 6 - 1,        // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-61)
    };

    rtc_set_time(&time_struct);

    ztimer_t alarm_timer;
    alarm_timer.callback = alarm_handler;
    alarm_timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &alarm_timer, 5000);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_ALARM) {
            printf("Alarm message received!\n");
        }
    }

    return 0;
}
