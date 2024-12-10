#include <ztimer.h>  // Include the ztimer header for timer functions
#include <stdio.h>   // Include standard input-output header for using printf
#include <gpio.h>    // Include GPIO header for controlling the LED

#define TIMER_INTERVAL (1 * ZTIMER_MSEC) // Define timer interval as 1 second
#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected to pin GPIO_PIN(0, 2)

static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    // Print message indicating LED state toggled
    printf("LED toggled\n");
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    ztimer_set_msg(&ztimer, TIMER_INTERVAL, timer_callback, NULL);

    // Keep the program running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}