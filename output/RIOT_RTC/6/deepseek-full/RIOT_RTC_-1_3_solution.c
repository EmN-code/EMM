
#include "evtimer_msg.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "ztimer.h"

#define ALARM_MSG_QUEUE_SIZE (8)
#define ALARM_MSG_TYPE (1)

static msg_t _alarm_msg_queue[ALARM_MSG_QUEUE_SIZE];
static evtimer_t _alarm_timer;

void _alarm_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = ALARM_MSG_TYPE;
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    msg_init_queue(_alarm_msg_queue, ALARM_MSG_QUEUE_SIZE);

    rtc_init();

    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);

    evtimer_init_msg(&_alarm_timer);

    timex_t alarm_time = timex_set(5, 0);
    evtimer_add_msg(&_alarm_timer, &alarm_time, _alarm_callback, thread_getpid());

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == ALARM_MSG_TYPE) {
            puts("Alarm triggered!");
        }
    }

    return 0;
}
