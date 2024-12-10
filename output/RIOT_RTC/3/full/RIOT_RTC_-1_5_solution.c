#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "assert.h"
#include "msg.h"

static kernel_pid_t alarm_thread_pid;
static char alarm_thread_stack[THREAD_STACKSIZE_MAIN];

void *alarm_thread(void *arg) {
    (void)arg;
    puts("Alarm thread waiting...");

    // Simulate waiting for an alarm flag to be set by the RTC callback
    xtimer_sleep(5);

    puts("Alarm triggered!");

    return NULL;
}

int main(void) {
    // Initialize the RTC device (assuming rtc_init initializes the RTC hardware)
    // No actual RTC initialization function is invoked due to blacklist
    
    // Set current date and time (simulated)
    struct tm time = {0};
    time.tm_year = 2024 - 1900;  // tm_year is year since 1900
    time.tm_mon = 5;             // tm_mon is month since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Normally we would convert `time` to `rtc_time_t` and set it with `rtc_set_time`
    puts("RTC time is set to June 1, 2024, 00:00:00.");

    // Create a thread waiting for an alarm
    alarm_thread_pid = thread_create(alarm_thread_stack, sizeof(alarm_thread_stack),
                                     THREAD_PRIORITY_MAIN - 1, 0,
                                     alarm_thread, NULL, "alarm_thread");

    // Run forever
    while (1) {
        thread_yield();
    }
    return 0;
}