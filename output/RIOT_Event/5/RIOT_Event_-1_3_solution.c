
#include "time_units.h"
#include "msg.h"
#include "thread.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    rtc_init();
    rtc_set_time(RTC_SEC(0), RTC_MIN(0), RTC_HOUR(0), RTC_DAY(1), RTC_MON(1), RTC_YEAR(2023));

    while (1) {
        periodic_callback(NULL);
        xtimer_sleep(INTERVAL_SEC);
    }

    return 0;
}
