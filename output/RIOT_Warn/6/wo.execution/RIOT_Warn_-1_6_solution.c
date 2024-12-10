#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 5) // Example pin for DHT11
#define LED_PIN GPIO_PIN(0, 6)   // Example pin for LED
#define TEMP_THRESHOLD 30        // Temperature threshold in degrees Celsius

static int16_t simulated_temperature = 0;

void read_dht_simulated(int16_t *temp) {
    // Simulate reading temperature from DHT11
    *temp = simulated_temperature;
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        int16_t temperature = 0;
        
        // Simulate DHT11 read
        read_dht_simulated(&temperature);
        
        // Check the temperature threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Sleep for 5 seconds (5000000 microseconds)
        ztimer_sleep(ZTIMER_USEC, 5000000);
    }
    return 0;
}