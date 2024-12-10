# Solution to set an RTC alarm on RIOT OS for ESP32 platform which triggers after 5 seconds set for a specified date.

// Description:
// This code initializes the RTC on ESP32 using RIOT OS, sets it to a specific date, and schedules
// an alarm to go off after 5 seconds. It will print "Alarm triggered!" when the alarm is activated.

#include "ztimer.h"  // Provides time and timer functionalities
#include "xtimer.h"  // Offers high level timers
#include "sys/types.h"  // Basic system data types
#include "stdio.h"  // Standard input/output
#include "stdatomic.h"  // Atomic operations in C
#include "stdlib.h"  // Standard library definitions
#include "stddef.h"  // Standard definitions
#include "stdbool.h"  // Boolean type
#include "netinet/in.h"  // Internet operations
#include "net/sock/util.h"  // Socket utility functions
#include "net/sock/udp.h"  // UDP socket layer
#include "net/emcute.h"  // Used for MQTT communication
#include "periph/rtc.h"  // RTC peripheral control, essential for using RTC
#include "periph/gpio.h"  // GPIO functionalities
#include "periph/i2c.h"  // I2C interface support
#include "msg.h"  // Messaging API
#include "mbox.h"  // Mailbox interface
#include "mutex.h"  // Mutex for resource locking
#include "kernel_defines.h"  // Definitions for kernel configs
#include "event/thread.h"  // Thread event operations
#include "evtimer_msg.h"  // Event timer using messages
#include "evtimer_mbox.h"  // Event timer using mailboxes
#include "arpa/inet.h"  // Definitions for internet operations
#include "time_units.h"  // Time-related operations
#include "MQTTClient.h"  // Provides MQTT functionalities
#include "paho_mqtt.h"  // Another MQTT library
#include "freertos/task.h"  // FreeRTOS task management
#include "esp_system.h"  // General ESP32 system functions
#include "unistd.h"  // UNIX standard definitions
#include "sys/socket.h"  // Socket operations

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    /* Initialize RTC */
    rtc_init();
    
    /* Set time to June 1, 2024 (year, month, day, hour, minute, second) */
    struct tm time = {
        .tm_year = 2024 - 1900,  // tm_year is years since 1900
        .tm_mon = 6 - 1,         // tm_mon is months since January
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&time);

    /* now get the current rtc time */
    struct tm now;
    rtc_get_time(&now);

    /* Print the current set RTC time for confirmation */
    printf("Current RTC time set: %s\n", asctime(&now));

    /* Set the alarm to trigger after 5 seconds */
    rtc_alarm_cb_t alarm;
    struct tm alarm_time = now;
    alarm_time.tm_sec += 5;  // Set it 5 seconds ahead
    alarm.time = mk_gmtime(&alarm_time);
    alarm.callback = alarm_callback;
    rtc_set_alarm(&alarm);

    /* Put the main thread to sleep to wait for the alarm */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}