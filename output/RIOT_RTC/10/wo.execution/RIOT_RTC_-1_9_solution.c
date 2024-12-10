#include <esp_system.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <msg.h>
#include <thread.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <zetimer.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <esp_wifi.h>
#include <mqtt/paho_mqtt.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <mutex.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <xtimer.h>

ztimer_t timer;

void set_alarm_handler(void *arg) {
    (void) arg; // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize ztimer
    ztimer_init(); // Initialization, required for ztimer to work correctly
    
    // Set current date/time (e.g., June 1, 2024 00:00:00)
    struct tm date;
    date.tm_year = 2024 - 1900; // tm_year is years since 1900
    date.tm_mon = 6 - 1;        // tm_mon is 0-based
    date.tm_mday = 1;
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;

    // You might need to convert date to your RTC implementation
    // Example only; dependent on the RTC implementation/driver used.
    // rtc_set_time(&date);

    // Configure the alarm, set it to trigger in 5 seconds from now
    ztimer_set(&timer, ZTIMER_MSEC, 5000, set_alarm_handler, NULL);

    // Keep the main thread running
    while (1) {
        // Use a delay to avoid busy waiting
        xtimer_sleep(1);
    }
    return 0;
}