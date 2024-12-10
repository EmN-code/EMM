
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "event/thread.h"

#define DHT_PIN            GPIO_PIN(0, 18)
#define LED_PIN            GPIO_PIN(0, 2)
#define TEMP_THRESHOLD     30

void read_dht(int16_t *temp, int16_t *hum) {
    // Simulated DHT reading function
    *temp = 25; // Simulated temperature value
    *hum = 50;  // Simulated humidity value
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is off initially

    while (1) {
        // Read DHT sensor
        read_dht(&temperature, &humidity);

        // Check if temperature exceeds threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED
        } else {
            gpio_clear(LED_PIN); // Turn off LED
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
