include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 1) //Specify the port and pin number of DHT11
#define LED_PIN GPIO_PIN(PORT_B, 2) //Specify the port and pin number of LED
#define TEMP_THRESHOLD 30 // temperature threshold in Celsius

void init_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    // Initialize other peripherals/devices if needed
}

int read_dht11_temp(void) {
    // Simulate read temperature from DHT11
    // In real implementation, read and return temperature from sensor
    return 25; // Simulated temperature
}

void check_temperature(void) {
    int temperature = read_dht11_temp();
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN); // Turn on LED if temperature is above the threshold
    } else {
        gpio_clear(LED_PIN); // Turn off LED if temperature is within limits
    }
}

int main(void) {
    init_gpio();
    while (1) {
        check_temperature();
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}