#include <timers/timer.h>
#include <peripherals/ESP32.h>
#include <drivers/led.h>

static tim_t timer_dev = 0;

void timer_callback(void *arg) {
    static bool led_on = false;
    if (led_on) {
        led_off();
        led_on = false;
    } else {
        led_on();
        led_on = true;
    }
}

int main(void) {
    /* Initialize the timer to trigger every 1 second */
    uint32_t freq = 1;  // 1 Hz frequency for 1-second intervals
    timer_init(timer_dev, freq, timer_callback, NULL);
    
    /* Start the timer */
    timer_start(timer_dev);

    while (1) {
        /* Keep the main thread alive */
    }

    return 0;
}