#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>  

// Define a simple callback function that will be called every second
void callback(void *arg) {
    (void)arg; // Avoid unused variable warning
    // Simple task performed every second
    gpio_toggle(GPIO_PIN(0, 2)); // Toggle LED on GPIO_PIN(0, 2)
}

int main(void) {
    // Set up GPIO for output
    gpio_init(GPIO_PIN(0, 2), GPIO_OUT);

    // Initialize a periodic timer
    tim_t dev = TIMER_DEV(0); // Assuming timer device 0
    uint32_t freq = 1; // Frequency of 1 Hz for one second intervals

    // Set up the timer with callback
    timer_init(dev, freq, callback, NULL);

    while(1) {
        // Main loop does nothing, everything is controlled by the timer
        msg_t msg;
        msg_receive(&msg); // Standby and wait for messages
    }

    return 0;
}