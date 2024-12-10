
#include "ztimer.h"
#include "timex.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "periph/i2c.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "net/emcute.h"
#include "periph/rtc.h"
#include "stdio.h"
#include "stdlib.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"
#include "stdbool.h"

#define GPIO_PIN (GPIO_PIN(0, 0))

static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void gpio_isr(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    gpio_init_int(GPIO_PIN, GPIO_IN, GPIO_RISING, gpio_isr, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Atomic variable value: %d\n", atomic_load(&atomic_var));
    }

    return 0;
}
