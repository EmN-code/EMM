
#include "xtimer.h"
#include "periph/rtc.h"
#include "msg.h"
#include "time_units.h"
#include "timex.h"
#include "mbox.h"
#include "thread.h"
#include "periph/i2c.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "periph/gpio.h"
#include "ztimer.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    xtimer_init();

    while (1) {
        periodic_callback(NULL);
        xtimer_sleep(INTERVAL_SEC);
    }

    return 0;
}
