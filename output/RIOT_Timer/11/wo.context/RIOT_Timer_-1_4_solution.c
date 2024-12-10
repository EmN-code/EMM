#include <xtimer.h>
#include <ztimer.h>
#include <event/callback.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2)    // Adjust the port and pin accordingly
#define TIMER_INTERVAL  (1U * US_PER_SEC)  // 1 second

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, toggling LED\n");
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize pin for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create and start the timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    xtimer_set(&timer, TIMER_INTERVAL);
    
    while (1) {
        // Main loop can perform other tasks
        xtimer_sleep(1);
    }
    return 0;
}