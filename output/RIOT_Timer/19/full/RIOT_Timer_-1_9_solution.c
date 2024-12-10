#include <xtimer.h>  // Include xtimer for timer functionality
#include <stdio.h>   // For input-output purposes
#include <event/callback.h>  // Include event/callback for handling events

#define TIMER_INTERVAL (1U * 1000000U)  // 1 second in microseconds
#define LED_PIN GPIO_PIN(0,2)  // Define the LED pin. Assuming pin 2 of port 0 is connected to the LED

static xtimer_t timer;
static int led_status = 0;  // To track the LED status (on/off)

void timer_callback(void *arg) {
    (void)arg;
    led_status = !led_status; // Toggle LED status
    if (led_status) {
        printf("LED is ON\n");
    } else {
        printf("LED is OFF\n");
    }
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    // Initialize the timer structure
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    printf("Starting LED Timer example\n");
    
    // Initialize timer
    xtimer_set(&timer, TIMER_INTERVAL);
    
    // Simulating the main loop to keep the program running
    while (1) {
        xtimer_sleep(2);  // Idle the main loop
    }
    return 0;
}