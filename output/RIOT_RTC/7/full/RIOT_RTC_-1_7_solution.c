#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define ALARM_INTERVAL (5U)

static evtimer_t evtimer;
static evtimer_msg_event_t event;
static msg_t msg;

void _alarm_handler(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    rtc_get_time(&time);
    time.tm_year = 2024 - 1900; /* Year is 2024 */
    time.tm_mon = 5; /* Month, June (0-based, so May = 5) */
    time.tm_mday = 1; /* Day */
    rtc_set_time(&time);

    evtimer_init(&evtimer, evtimer_msg_event);
    evtimer_msg_event_init( &event.event, _alarm_handler, NULL );
    evtimer_add_msg(&evtimer, &event.event, &msg, ALARM_INTERVAL);

    while (1) {
        msg_receive(&msg);
    }

    return 0;
}