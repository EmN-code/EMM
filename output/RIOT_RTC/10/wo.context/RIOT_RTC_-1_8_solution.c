#include <xtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (5U*US_PER_SEC) // 5 seconds interval

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Setup RTC or any required date setting here
    // For this example, actual RTC setting code is assumed to be elsewhere

    xtimer_t timer;
    timer.callback = alarm_callback;
    timer.arg = NULL;

    // Convert 5 seconds delay to timer ticks
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}