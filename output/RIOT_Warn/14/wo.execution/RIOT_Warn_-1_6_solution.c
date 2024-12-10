#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "periph/gpio.h"

#define DHT_PIN            GPIO_PIN(0, 0)  // Example pin, adjust the pin number accordingly
#define LED_PIN            GPIO_PIN(0, 1)  // Example pin, adjust for your setup
#define TEMP_THRESHOLD     300             // Temperature threshold (in tenths of a degree Celsius)

int main(void)
{
    // Initialize the DHT sensor
    dht_t dev;
    int16_t temperature, humidity;

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    dht_init(&dev, DHT_PIN, DHT11);

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != ESP_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, abs(temperature % 10), 
                   humidity / 10, abs(humidity % 10));

            // Check if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn LED on
            } else {
                gpio_clear(LED_PIN);  // Turn LED off
            }
        }

        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}