#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"
#include "mutex.h"

void* alarm_thread(void *arg) {
    (void)arg;
    // Sleep to simulate waiting for the alarm
    xtimer_sleep(5);
    puts("Alarm triggered!");
    return NULL;
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the desired date and time
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;

    // Set RTC time
    assert(rtc_set_time(mktime(&time)) == 0);

    // Define thread variables
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_pid;

    // Create a thread for the alarm
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, alarm_thread, NULL, "alarm_thread");
    assert(thread_pid > 0);

    // Main loop, or perform other tasks
    while (1) {
        thread_yield();
    }
    return 0;
}