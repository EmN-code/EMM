#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "unistd.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_struct;
    uint16_t milliseconds;

    // Set date to June 1, 2024
    time_struct.tm_year = 2024 - 1900;  // year since 1900
    time_struct.tm_mon = 5;             // June (0-based index)
    time_struct.tm_mday = 1;            // 1st day
    time_struct.tm_hour = 0;
    time_struct.tm_min = 0;
    time_struct.tm_sec = 0;

    rtc_set_time(&time_struct);

    // Set alarm for 5 seconds later
    milliseconds = 5000;
    ztimer_t timer;
    timer.callback = alarm_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, milliseconds);

    return 0;
}