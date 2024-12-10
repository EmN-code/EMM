#include "esp_system.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h" // rtc functions
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "ztimer.h"

static void alarm_triggered_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // error correction layer
    while (rtc_init() != 0) {
        printf("RTC init failed. Retrying...\n");
        sleep(1);
    }

    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon  = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    // error correction layer
    while (rtc_set_time(&time) != 0) {
        printf("Setting RTC time failed. Retrying...\n");
        sleep(1);
    }

    // set alarm for 5 seconds later
    rtc_set_alarm_cb_t cb_func = alarm_triggered_cb;
    while (rtc_set_alarm(&time, cb_func, NULL) != 0) {
        printf("Setting RTC alarm failed. Retrying...\n");
        sleep(1);
    }

    return 0;
}