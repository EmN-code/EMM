#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 4)  // Customize the pin based on your ESP32 setup
#define LED_PIN         GPIO_PIN(0, 2)  // Customize the pin based on your ESP32 setup
#define TEMP_THRESHOLD  30

void init_sensors(void) {
    int result = dht_init(&dht, &gpio_dev(DHT_PIN));
    if (result != DHT_OK) {
        printf("Failed to init DHT sensor\n");
        return;
    }
}

void read_temperature_and_control_led(void) {
    int16_t temperature, humidity;
    int dht_ret;

    while (1) {
        xtimer_sleep(5);

        dht_ret = dht_read(dht, &temperature, &humidity);
        if (dht_ret != DHT_OK) {
            printf("Error reading DHT sensor\n");
            continue;
        }

        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    init_sensors();
    read_temperature_and_control_led();

    return 0;
}