#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10) // Correct GPIO definition for DHT
#define LED_PIN GPIO_PIN(PORT_A, 9) // Correct GPIO definition for LED
#define TEMP_THRESHOLD 30 // Define the temperature threshold

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int res;

    // Initialize the temperature sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: Cannot initialize LED pin");
        return 1;
    }

    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C Humidity: %d.%d%%
",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Turn on the LED if the temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor, error %d
", res);
        }

        ztimer_sleep(ZTIMER_SEC, 5); // Sleep for 5 seconds
    }
    return 0;
}