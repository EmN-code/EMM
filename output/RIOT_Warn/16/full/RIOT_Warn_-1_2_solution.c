#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define SENSOR_PIN GPIO_PIN(0, 5)  // Example pin, change as needed
#define LED_PIN GPIO_PIN(0, 6)     // Example pin, change as needed
#define TEMP_THRESHOLD 30

dht_t sensor;

void init_sensor() {
    if (dht_init(&sensor, SENSOR_PIN, DHT11) != 0) {
        printf("Error initializing sensor\n");
    }
}

void check_temperature() {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&sensor, &temp, &hum) != 0) {
            printf("Error reading sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensor();
    check_temperature();
    return 0;
}