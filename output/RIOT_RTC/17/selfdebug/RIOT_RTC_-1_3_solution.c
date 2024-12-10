#include <xtimer.h>
#include <ztimer.h>
#include <stdio.h>
#include <event/callback.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC alarm for 5 seconds from now...\n");
    
    ztimer_sleep(ZTIMER_USEC, 5000000); // sleep for 5 seconds
    alarm_callback(NULL);  // calling alarm callback
    
    return 0;
}