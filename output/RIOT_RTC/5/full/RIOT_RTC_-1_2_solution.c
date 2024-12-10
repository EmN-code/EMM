#include <ztimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC simulation date to June 1, 2024...\n");
    printf("Alarm to trigger in 5 seconds...\n");
    
    // Simulate delay using ztimer - Note: ztimer waits in milliseconds
    ztimer_sleep(ZTIMER_MSEC, 5000);  // 5 seconds delay

    alarm_callback();  // Simulates the alarm being triggered

    return 0;
}