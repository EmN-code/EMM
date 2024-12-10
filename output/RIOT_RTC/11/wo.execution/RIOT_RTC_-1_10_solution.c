#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    rtc_time_t time;
    time.year = 2024;
    time.month = 6;
    time.day = 1;
    time.hour = 0;
    time.minute = 0;
    time.second = 0;

    rtc_set_time(&time);

    // Schedule alarm 5 seconds later
    rtc_set_alarm_cb(alarm_callback, NULL);
    rtc_time_t alarm_time = time;
    alarm_time.second += 5;
    if (alarm_time.second >= 60) {
        alarm_time.second -= 60;
        alarm_time.minute += 1;
    }
    rtc_set_alarm(&alarm_time);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}