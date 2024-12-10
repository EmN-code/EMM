#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"
#include <stdint.h>

#define DHT_PIN 4        // Define GPIO pin for DHT sensor
#define LED_PIN 5        // Define GPIO pin for LED
#define TEMP_THRESHOLD 30 // Define temperature threshold

int read_sensor_data(int16_t *temp, int16_t *humidity) {
    dht_t dev;
    // Initialize DHT sensor device 
    if (dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN, DHT_PARAM::USE_DELAY_BUS) != 0) {
        printf("DHT Sensor initialization failed!\n");
        return -1;
    }

    // Read data from sensor
    if (dht_read(&dev, temp, humidity) != 0) {
        printf("Failed to read from DHT sensor\n");
        return -1;
    }
    return 0;
}

void control_led(int16_t temp) {
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    while (1) {
        int16_t temp = 0, humidity = 0;

        // Read sensor data
        if (read_sensor_data(&temp, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, humidity);

            // Control the LED based on temperature
            control_led(temp);
        } else {
            printf("Sensor read error\n");
        }

        // Wait for 5 seconds before next read
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}