#include "xtimer.h"
#include "event/callback.h"
#include "event/thread.h"
#include "periph/gpio.h"

// Callback function for the alarm
void alarm_callback(void *arg) {
    (void)arg;  // Argument is unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Using xtimer as a substitute to wait 5 seconds
    uint32_t waiting_time = 5 * 1000000; // 5 seconds in microseconds
    xtimer_sleep_usec(waiting_time);
    
    // Trigger the alarm
    alarm_callback(NULL);
    
    return 0;
}