// This C code demonstrates configuring an RTC alarm on ESP32 using the RIOT OS

#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "time_units.h"

void *alarm_thread(void *arg) {
    (void)arg;
    // Alarm thread waits for a signal and then prints when the alarm is triggered
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);
        puts("Alarm triggered!");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    struct tm time;
    rtc_init();
    
    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;           // Month, where 0 = January
    time.tm_mday = 1;          // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    if (rtc_set_time(&time) != 0) {
        puts("Error setting RTC time");
        return 1;
    }

    // Set alarm for 5 seconds from the current time
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;
    if (rtc_set_alarm(&alarm_time, alarm_thread) != 0) {
        puts("Error setting RTC alarm");
        return 1;
    }

    // Start a thread to handle the alarm
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, alarm_thread, NULL, "alarm_thread");

    return 0;
}