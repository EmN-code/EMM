#include <string.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <thread.h>

// Set the GPIO pins for DHT11 and LED
#define DHT_GPIO 4 // Example GPIO pin for DHT11
#define LED_GPIO 2 // Example GPIO pin for LED

// Temperature threshold
#define TEMP_THRESHOLD 30 // Example temperature threshold in degree Celsius

// Timer settings
#define SLEEP_INTERVAL (1000000 * 5) // 5 seconds, converted to microseconds for xtimer

void warn_if_temp_exceeds(int16_t temp) {
    if (temp > TEMP_THRESHOLD) {
        // Turn on the LED by setting the GPIO high
        gpio_set(LED_GPIO);
    } else {
        // Turn off the LED
        gpio_clear(LED_GPIO);
    }
}

int main(void) {
    dht_t dht_dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    dht_dev.pin = DHT_GPIO;
    dht_dev.type = DHT11;

    // Initialize the LED pin as output
    gpio_init(LED_GPIO, GPIO_OUT);

    while (1) {
        // Read the temperature and humidity from the DHT11 sensor
        int result = dht_read(&dht_dev, &temp, &hum);
        if (result == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d\n", temp / 10, temp % 10, hum / 10, hum % 10);
            warn_if_temp_exceeds(temp);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}