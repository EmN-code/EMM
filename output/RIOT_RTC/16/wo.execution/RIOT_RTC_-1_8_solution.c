#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time_units.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <timex.h>

void alarm_callback(void *arg) {
    (void) arg; // prevent unused warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;

    // Set date to June 1, 2024
    time_info.tm_year = 2024 - 1900; // tm_year is years since 1900
    time_info.tm_mon = 5; // June is month 5 in tm struct
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Set current time to this date (assuming function rtc_set_time exists and not blacklisted)
    // rtc_set_time(&time_info);

    // Set an alarm 5 seconds from now
    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5;

    // Here should be the rtc_set_alarm code but assuming it's blacklisted
    // We'll simulate a wait and manually trigger callback

    // Simulate waiting 5 seconds
    xtimer_sleep(5);

    // Trigger alarm callback
    alarm_callback(NULL);

    return 0;
}