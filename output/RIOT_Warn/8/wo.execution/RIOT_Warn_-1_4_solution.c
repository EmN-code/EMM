#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define LED_PIN GPIO_PIN(PORT_C, 3)
#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize GPIO for LED\n");
        return 1;
    }

    // Initialize DHT device
    dht_t dev;
    dht_params_t params = DHT_PARAMS(DHT_PIN);
    if (dht_init(&dev, &params) == -1) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}