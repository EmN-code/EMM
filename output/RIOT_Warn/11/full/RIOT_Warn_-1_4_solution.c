#include "thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/i2c.h"
#include <MQTTClient.h>

#define DHT11_PIN GPIO_PIN(0, 4)  // Example GPIO pin
#define LED_PIN GPIO_PIN(0, 5)    // Example GPIO pin for LED
#define TEMP_THRESHOLD 30         // Temperature threshold for warning

void init_sensors(void) {
    // Initialize DHT
    dht_t dev;
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize the DHT sensor\n");
        return;
    }
    
    // Initialize LED
    gpio_init(LED_PIN, GPIO_OUT);
}

bool read_temperature(dht_t *dev, int16_t *temperature, int16_t *humidity) {
    esp_err_t res;
    while (1) {
        res = dht_read(dev, temperature, humidity);
        if (res == DHT_OK) {
            return true;
        } else {
            printf("Failed to read from DHT sensor\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }
}

void check_temperature_warning(int16_t temperature) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on LED
    } else {
        gpio_clear(LED_PIN);  // Turn off LED
    }
}

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    init_sensors();

    while (1) {
        if (read_temperature(&dev, &temperature, &humidity)) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            check_temperature_warning(temperature);
        }
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}