#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "unistd.h"
#include "thread.h"

#define ALARM_DELAY 5  // 5 seconds delay for RTC alarm

/* Alarm function to print message */
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

/* Simulate setting time and using ztimer for delay */
void setup_rtc_and_alarm(void) {
    /* Assuming RTC time setting to June 1, 2024 */
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    /* Simulate RTC set time with ztimer sleep to set an alarm */
    ztimer_sleep(ZTIMER_USEC, ALARM_DELAY * US_PER_SEC);
    alarm_callback();
}

int main(void) {
    setup_rtc_and_alarm();
    return 0;
}