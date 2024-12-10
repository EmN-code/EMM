/* The following RIOT-OS C application runs on ESP32 and sets a RTC alarm. 
 * It initializes the RTC with a given time and sets an alarm to trigger 
 * after 5 seconds, at which point it prints "Alarm triggered!" to the console.
 */

// Include necessary header files in descending alphabetical order as per the requirement
#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "rtc.h"
#include "periph/rtc.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "kernel.h"
#include "isrpipe.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "dht_params.h"
#include "stddef.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "assert.h"
#include <time.h>

// Function to be called when the alarm triggers
void alarm_triggered_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t cb = alarm_triggered_callback;
    
    // Error handling in RTC initialization
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5;  // tm_mon is 0-indexed, hence 5 for June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm to trigger after 5 seconds
    time.tm_sec = 5;

    // Error handling in setting RTC alarm
    while (rtc_set_alarm(&time, cb, NULL) != 0) {
        printf("RTC set alarm failed. Retrying...\n");
        xtimer_sleep(1);  // Retry every 1 second
    }

    printf("Alarm set for 5 seconds from now\n");

    // Suspend main thread to keep program running
    msg_t msg;
    msg_receive(&msg);
    
    return 0;
}