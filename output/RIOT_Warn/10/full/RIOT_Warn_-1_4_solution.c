// Include necessary header files including the ones specified and maintaining descending order
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"

// Function prototype
void read_dht_sensor(void);

// Initialize global variables
int16_t temperature = 0;
int16_t humidity = 0;

// Main function
int main(void) {
    // Declare variables for the DHT sensor
    dht_t dev;
    gpio_t warning_led = GPIO_PIN(0, 1);  // Assume using GPIO port 0, pin 1
    int threshold_temperature = 30;        // Temperature threshold for LED warning
    
    // Setup the warning LED GPIO pin
    gpio_init(warning_led, GPIO_OUT);

    // Repeatedly read the sensor and check the temperature
    while (1) {
        // Read temperature and humidity from DHT sensor
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);  // Printing the values

            // Check if temperature exceeds threshold
            if (temperature >= threshold_temperature * 10) {  // DHT sensor provides temperature*10
                gpio_set(warning_led);  // Turn on LED
            } else {
                gpio_clear(warning_led);  // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor.\n");
        }

        // Delay for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}