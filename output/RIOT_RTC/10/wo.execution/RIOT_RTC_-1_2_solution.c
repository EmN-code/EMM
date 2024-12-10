#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include "event/thread.h"
#include "thread.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC alarm...\n");
    
    // Simulating setting a date and time
    // Normally, we would initialize the RTC and set the current date/time here
    // For the purpose of this example, we'll simulate an alarm trigger

    // Simulate waiting of 5 seconds for an alarm
    usleep(5 * 1000000);

    // Trigger the alarm callback
    alarm_callback(NULL);

    return 0;
}