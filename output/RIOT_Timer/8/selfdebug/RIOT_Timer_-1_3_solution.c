#include <stdio.h>
#include <timex.h>
#include <timer.h>

#define LED_PIN 2  // Define the LED pin number

static void timer_callback(void *arg) {
    (void)arg;
    // Code to turn on LED
    // Example: gpio_set(LED_PIN);
    printf("LED turned on\n");
}

int main(void) {
    printf("Timer test on ESP32\n");

    timer_t timer;
    timer_init(&timer, 1, timer_callback, NULL);

    while (1) {
        // Idle loop to keep main running
    }
    return 0;
}