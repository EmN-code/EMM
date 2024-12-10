#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 10) // Define the pin the DHT is connected to
#define LED_PIN GPIO_PIN(PORT_B, 0) // Define the pin for the LED
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning
#define SLEEP_INTERVAL_MS (5U * 1000U)  // 5 seconds interval

dht_t dev;

void init_sensors(void) {
    dev.params.pin = DHT_PIN;
    dht_init(&dev, DHT11);
    gpio_init(LED_PIN, GPIO_OUT);
}

int main(void) {
    init_sensors();

    while (1) {
        int16_t temp, hum;

        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {  // Temperature is multiplied by 10 by the sensor
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            puts("Error reading from sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL_MS / 1000);
    }

    return 0;
}