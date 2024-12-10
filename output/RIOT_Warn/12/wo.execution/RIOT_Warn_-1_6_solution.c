#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 3) // Example pin for DHT
#define LED_PIN GPIO_PIN(0, 2) // Example pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold

static dht_t dev;

void initialize(void) {
    // Initialize DHT
    dht_init(&dev, &dht_params[0]);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

int main(void) {
    int16_t temperature;
    int16_t humidity;

    initialize();

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            // Convert temperature to degrees Celsius
            int temp_celsius = temperature / 10;
            
            // Check if temperature exceeds threshold
            if (temp_celsius > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        
        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_USEC, 5000000);
    }

    return 0;
}