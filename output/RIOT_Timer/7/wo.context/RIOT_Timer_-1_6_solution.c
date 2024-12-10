#include <xtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    printf("LED ON\n");
    // Code to turn on the LED
}

int main(void)
{
    puts("Starting timer example on ESP32 using RIOT");
    
    // Initialize the timer structure
    xtimer_set_callback(&timer, TIMER_INTERVAL, timer_callback, NULL);
    
    // Creating the loop to give time to process other tasks
    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
        // Other code can be executed here
    }
    return 0;
}