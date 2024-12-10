#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "unistd.h"
#include "periph/gpio.h"
#include "msg.h"
#include "ztimer_now.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "sys/socket.h"
#include "mbox.h"
#include "mutex.h"
#include "periph/i2c.h"
#include "kernel_defines.h"
#include "periph/rtc.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"

#define LED_PIN GPIO_PIN(0, 2)
#define INTERVAL (1U * US_PER_SEC)

static ztimer_periodic_t timer;

void timer_handler(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_periodic_init(ZTIMER_USEC, &timer, timer_handler, NULL, ztimer_usec_from_sec(1));
    ztimer_periodic_start(&timer);

    while (1) {
        /* Let the timer run and toggle LED */
        sleep(1);
    }
    return 0;
}