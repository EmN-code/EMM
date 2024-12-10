#include "event/thread.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"

#define TEMP_THRESHOLD 30 // Threshold for temperature in Celsius
#define GPIO_LED_PIN 2 // GPIO pin for LED
#define DHT_PIN 0 // GPIO pin connected to DHT11

int main(void) {
    // Initialize DHT sensor
    dht_t dht_dev;
    dht_init(&dht_dev, DHT_PIN);

    // Initialize LED pin
    gpio_init(GPIO_LED_PIN, GPIO_OUT);

    int16_t temperature, humidity;
    
    while (1) {
        // Read values from DHT sensor
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(GPIO_LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(GPIO_LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}