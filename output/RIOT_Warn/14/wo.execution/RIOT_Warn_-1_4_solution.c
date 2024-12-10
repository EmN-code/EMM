#include "mma8x5x_regs.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"
#include "mma8x5x.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Define your actual PORT and PIN
#define LED_PIN GPIO_PIN(PORT_A, 1)   // Define your actual PORT and PIN
#define TEMP_THRESHOLD 30             // Set temperature threshold for LED warning

void main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // DHT initialization
    dht_params_t dht_params;
    dht_params.pin = DHT11_PIN;
    dht_t dht_device;
    dht_init(&dht_device, &dht_params);

    while (1) {
        int16_t temperature, humidity;

        if (dht_read(&dht_device, &temperature, &humidity) == 0) {
            printf("Temp: %d, Hum: %d\n", temperature, humidity);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading from DHT sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}