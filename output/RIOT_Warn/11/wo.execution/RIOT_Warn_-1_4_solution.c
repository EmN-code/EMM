#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "thread.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define SENSOR_PIN 5
#define LED_PIN 2
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning
#define SLEEP_INTERVAL 5   // Sleep interval in seconds

gpio_t sensor_pin = SENSOR_PIN;
gpio_t led_pin = LED_PIN;

int main(void) {
    // Initialize the sensor and LED pins
    if (gpio_init(sensor_pin, GPIO_IN) != 0) {
        // Initialization failed
    }
    if (gpio_init(led_pin, GPIO_OUT) != 0) {
        // Initialization failed
    }

    dht_t dev;
    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_params_t params;

    // Initialize DHT11 sensor structure
    params.pin = sensor_pin;
    params.type = DHT11;

    while (1) {
        // Read from the sensor
        if (dht_read(&dev, &temperature, &humidity) != ESP_OK) {
            // Handle error
        }
        
        // Check the temperature and turn on the LED if threshold exceeded
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(led_pin);
        } else {
            gpio_clear(led_pin);
        }

        // Sleep for defined interval
        thread_sleep(SLEEP_INTERVAL);
    }
    return 0;
}