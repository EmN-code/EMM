#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <ztimer.h>
#include <ztimer_sleep.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <stdio.h>
#include <esp_system.h>

void alarm_callback(void *arg){
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void){
    // Initialize RTC and set date to June 1, 2024
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 5; // 0-indexed: Jan=0, ..., June=5
    timeinfo.tm_mday = 1; // First day of the month
    time_t t_of_day = mktime(&timeinfo);    

    // Set an alarm for 5 seconds from now
    evtimer_msg_event_t event_alarm;
    event_alarm.event.handler = alarm_callback;
    ztimer_set(ZTIMER_SEC, &event_alarm.event, 5);
   
    while (1) {
        thread_sleep();
    }

    return 0;
}