# Include required headers for RIOT, networking, and peripherals
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "stdatomic.h"

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC and set the date
    struct tm time_info;
    time_info.tm_year = 2024 - 1900;  // Since tm_year is years since 1900
    time_info.tm_mon = 6 - 1;         // Months are 0-11, so June is 5
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    rtc_get_time(&time_info);

    // Set the alarm for 5 seconds in the future
    ztimer_sleep(ZTIMER_MSEC, 5000);
    
    // Set callback for the alarm
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t target_pid = thread_getpid();
    msg_t msg;
    evtimer_init_msg(&evtimer);
    event.msg.type = -1;  // Just a placeholder type
    evtimer_add_msg(&evtimer, &event, target_pid);
    while (1) {
        msg_receive(&msg);
        if (msg.type == -1) {  // Check if it's our alarm message
            alarm_callback(NULL);
        }
    }

    return 0;
}