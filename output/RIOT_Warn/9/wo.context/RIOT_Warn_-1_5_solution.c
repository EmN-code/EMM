#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO port number correctly
#define LED_PIN GPIO_PIN(PORT_B, 1) // Define the GPIO port number correctly
#define THRESHOLD_TEMP 30  // Temperature threshold to turn on the LED

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    // Set the sensor type directly if not defined by a macro
    params.type = DHT11;

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    int16_t temp, hum;
    while (1) {
        if (dht_read(&params, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", (temp / 10), (temp % 10), (hum / 10), (hum % 10));

            if (temp >= (THRESHOLD_TEMP * 10)) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading sensor\n");
        }

        xtimer_sleep(5);
    }
    return 0;
}