#include <arpa/inet.h>  // Required for inet_addr functions
#include <dht.h>  // Used for DHT sensors
#include <dht_params.h>  // Parameters for DHT
#include <evtimer_msg.h>  // For event timer messaging
#include <evtimer_mbox.h>  // Event timer mbox
#include <kernel_defines.h>  // Kernel definitions
#include <mbox.h>  // Mailbox for inter-thread communication
#include <mma8x5x.h>  // For MMA8x5x sensor family
#include <mma8x5x_params.h>  // Parameters for MMA8x5x
#include <mma8x5x_regs.h>  // Registers for MMA8x5x
#include <msg.h>  // Messaging
#include <mutex.h>  // For mutex locks
#include <net/emcute.h>  // Lightweight MQTT-SN
#include <net/gcoap.h>  // CoAP protocol
#include <net/sock/udp.h>  // UDP networking
#include <net/sock/util.h>  // Socket utilities
#include <netinet/in.h>  // For internet addresses
#include <stdbool.h>  // For boolean type
#include <stddef.h>  // Standard definitions
#include <stdint.h>  // Standard integer types
#include <stdio.h>  // Standard I/O functions
#include <stdlib.h>  // Standard library
#include <stdatomic.h>  // Atomic operations
#include <string.h>  // String operations
#include <sys/socket.h>  // Socket operations
#include <timex.h>  // Time types
#include <time_units.h>  // Time units conversion
#include <unistd.h>  // UNIX standard functions
#include <xtimer.h>  // Extended timer
#include <ztimer.h>  // Ztimer library

#define ALARM_DELAY_SEC (5) // 5 seconds delay

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm current_time, alarm_time;

    // Set current date to June 1, 2024
    current_time.tm_year = 2024 - 1900;  // Year since 1900
    current_time.tm_mon  = 6 - 1;        // Month, 0 - January
    current_time.tm_mday = 1;            // Day of the month
    current_time.tm_hour = 0;
    current_time.tm_min  = 0;
    current_time.tm_sec  = 0;

    // Set the RTC current time
    if (rtc_set_time(&current_time) != 0) {
        printf("Error setting RTC time!\n");
        return 1;
    }

    // Set alarm time 5 seconds from now
    alarm_time = current_time;
    alarm_time.tm_sec += ALARM_DELAY_SEC;

    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm!\n");
        return 1;
    }

    printf("RTC Clock is set. Alarm will be triggered in %d seconds.\n", ALARM_DELAY_SEC);

    return 0;
}