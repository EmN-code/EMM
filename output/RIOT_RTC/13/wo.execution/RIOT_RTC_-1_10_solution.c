#include "ztimer.h"
#include "timex.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "sys/socket.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "time_units.h"

#define ALARM_OFFSET 5

static void _alarm_triggered(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the system
    printf("Setting up RTC and alarm...\n");

    // Assuming rtc_init initializes RTC
    rtc_init();

    // Set a date (let’s simulate)
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 5;  // June
    timeinfo.tm_mday = 1;
    rtc_set_time(&timeinfo);

    // Set an alarm for 5 seconds later
    ztimer_t alarm_timer;
    ztimer_set_timeout(ZTIMER_MSEC, &alarm_timer, 5 * 1000);
    alarm_timer.callback = _alarm_triggered;

    return 0;
}