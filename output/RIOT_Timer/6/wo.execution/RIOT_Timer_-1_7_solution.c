#include <xtimer.h>   // For timer operations
#include <stdio.h>    // For printf operations

// LED Pin (Assuming GPIO_PIN function will correctly parse the right pin)
#define LED_PIN GPIO_PIN(1, 0)

// Timer configuration
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second in microseconds

static xtimer_t timer;

// Dummy function to simulate LED set (Assume such function exists)
void gpio_set(int pin) {
    printf("Turning on LED on pin %d\n", pin);  // Simulating the LED being turned on.
}

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, toggling LED\n");
    gpio_set(LED_PIN);
}

int main(void) {
    // Initialize and set up the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}