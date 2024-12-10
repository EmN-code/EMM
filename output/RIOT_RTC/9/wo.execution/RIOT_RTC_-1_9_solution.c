#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/socket.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <unistd.h>

void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set date and time to 2024-06-01 00:00:00
    struct tm time = {0};
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    if (rtc_set_time(mktime(&time)) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set an alarm to trigger after 5 seconds
    ztimer_sleep(ZTIMER_USEC, 5 * 1000000);  // Wait for 5 seconds
    alarm_cb(NULL);

    return 0;
}