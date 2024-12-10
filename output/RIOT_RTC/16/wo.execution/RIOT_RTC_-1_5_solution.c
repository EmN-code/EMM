#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"

// Define RTC alarm callback function
void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    struct tm time_struct;
    evtimer_msg_event_t alarm_event;

    // Initialize event timer for messages
    evtimer_init_msg(&evtimer);

    // Set the date to June 1, 2024
    time_struct.tm_year = 2024 - 1900;  // years since 1900
    time_struct.tm_mon = 6 - 1;         // months since January
    time_struct.tm_mday = 1;
    time_struct.tm_hour = 0;
    time_struct.tm_min = 0;
    time_struct.tm_sec = 0;

    // Set RTC time here (use appropriate functions or system calls depending on your system's RTC API)
    // Assuming a functional equivalent of rtc_set_time(), use this placeholder
    // rtc_set_time(&time_struct);

    // Prepare event for when the alarm triggers
    kernel_pid_t pid = 0;  // Assume the main thread's PID for receiving the message
    alarm_event.msg.content.value = 1;  // Value to be sent with the event

    // Add alarm event to the event timer, assuming 5 seconds for the alarm
    evtimer_add_msg(&evtimer, &alarm_event, pid);

    while (1) {
        msg_t msg;

        // Wait indefinitely for messages
        if (msg_receive(&msg) == 1) {
            // Check if it's our timer message (use your own logic/message content)
            if (msg.content.value == 1) {
                rtc_alarm_callback(NULL);
            }
        }
    }

    return 0;
}