#include "xtimer.h"
#include "shell.h"
#include "saul_reg.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define THRESHOLD_TEMP 30 // temperature threshold in Celsius
#define LED_PIN GPIO_PIN(PORT_B, 5) // Assume LED is connected to this GPIO pin

int main(void) {
    saul_reg_t *dev = saul_reg_find_nth(0); // Assumes the first device is the DHT11
    if (!dev) {
        printf("DHT11 sensor not found\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        phydat_t res;

        int result = saul_reg_read(dev, &res);
        if (result < 0) {
            printf("Error reading from DHT11\n");
        } else {
            int temperature = res.val[0];
            printf("Temperature: %d°C\n", temperature);

            if (temperature > THRESHOLD_TEMP) {
                printf("Temperature exceeds threshold! Turning on LED.\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5); // delay 5 seconds
    }

    return 0;
}