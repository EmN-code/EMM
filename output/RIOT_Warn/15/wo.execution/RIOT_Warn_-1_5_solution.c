#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"

#define GPIO_DHT11_PIN    4  // GPIO pin for DHT11
#define GPIO_LED_PIN      2  // GPIO pin for the LED
#define TEMP_THRESHOLD    30 // Temperature threshold for warning (in Celsius)
#define SLEEP_INTERVAL    (5 * 1000000U) // Read every 5 seconds (in microseconds)

void read_dht11_and_control_led(void) {
    // Initialize GPIO for LED
    gpio_init(GPIO_LED_PIN, GPIO_OUT);

    int16_t temp;
    int16_t hum;
    gpio_t pin = GPIO_DHT11_PIN;
    gpio_set(pin);
    
    while (1) {
        // Read DHT11 sensor data
        if (dht_read(pin, &temp, &hum) == DHT_OK) {
            // Check if temperature exceeds threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(GPIO_LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(GPIO_LED_PIN);
            }
        }

        // Wait for the next reading
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Call the function to read DHT11 and control LED
    read_dht11_and_control_led();
    return 0;
}
