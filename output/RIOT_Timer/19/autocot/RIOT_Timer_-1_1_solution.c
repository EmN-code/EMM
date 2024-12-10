#include <stdio.h>
#include <stdint.h>
#include <tim.h>
#include <uart.h>

#define TIMER_FREQUENCY 1000  // Frequency in Hz

static tim_t timer_dev = 0;  // The timer device to use
static uart_t uart_dev = 0;  // UART device, dev 0 used for demonstrating

// Timer callback function
void timer_callback(void *arg) {
    const uint8_t led_on_message[] = "LED ON\n";
    uart_write(uart_dev, led_on_message, sizeof(led_on_message) - 1);
    // Reset the timer counting
timer_init(timer_dev, TIMER_FREQUENCY, timer_callback, NULL);
    timer_start(timer_dev);
}

int main(void) {
    puts("Starting timer example...");

    // Initialize UART (assumed initialized for demo purposes)
    // Initialize Timer
    if (timer_init(timer_dev, TIMER_FREQUENCY, timer_callback, NULL) < 0) {
        puts("Error initializing timer");
        return -1;
    }

    // Start Timer
    timer_start(timer_dev);

    // Keep the program running
    while (1) {}

    return 0;
}