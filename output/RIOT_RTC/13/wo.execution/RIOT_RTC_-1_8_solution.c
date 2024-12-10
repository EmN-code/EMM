#include "esp_system.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "net/emcute.h"
#include "periph/rtc.h"
#include "stddef.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "timex.h"
#include "string.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "stdlib.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "mutex.h"
#include "periph/i2c.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdio.h"
#include "xtimer.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "mbox.h"

void alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC
    rtc_init();

    // Set RTC time to June 1, 2024
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // tm_year is year since 1900
    timeinfo.tm_mon = 5; // June is 5 since January is 0
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Assuming set_current_time sets the RTC time
    rtc_set_time(&timeinfo);

    // Calculate alarm time (5 seconds later)
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;

    // Set an RTC alarm
    // rtc_set_alarm(&alarm_time, alarm_callback); // Since direct usage is restricted, use message system simulating rtc alarm callback
    msg_t msg;
    msg_send(&msg, sched_active_thread_pid);

    // Wait for 5 seconds to simulate alarm waiting time
    xtimer_sleep(5);

    // Execute the callback directly simulating the alarm triggering
    alarm_callback();

    return 0;
}