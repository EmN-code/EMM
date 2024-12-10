#include <timex.h>
#include <timer.h>
#include <board.h>

// Timer callback function
static void timer_callback(void *arg) {
    (void)arg;
    // Code to toggle LED
    LED_ON();
    xtimer_usleep(100000);  // Keep LED on for 100ms
    LED_OFF();
}

int main(void) {
    // Initialize board and LED
    board_init();
    LED_INIT();

    // Initialize the timer
    tim_t timer_dev = TIMER_DEV(0);  // Use timer 0
    uint32_t freq = 1;               // Set frequency to 1Hz
    timer_init(timer_dev, freq, timer_callback, NULL);

    // Start the timer
    timer_start(timer_dev);

    while (1) {
        // Main loop
    }
    return 0;
}