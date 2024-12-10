#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "event/thread.h"

#define TEMP_THRESHOLD (25)
#define DHT_PIN GPIO_PIN(0, 4)  // Example pin
#define LED_PIN GPIO_PIN(0, 2)  // Example pin

int main(void) {
    int16_t temp, hum;
    dht_t dev;

    // Initialize the DHT11 sensor
    while (dht_init(&dev, &dht_params[0]) != 0) {
        printf("DHT initialization failed\n");
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Main loop
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
            
            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}