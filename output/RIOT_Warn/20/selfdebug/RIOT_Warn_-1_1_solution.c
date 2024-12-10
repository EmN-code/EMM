#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 7)
#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TEMP_THRESHOLD 30
#define DELAY 5

dht_t dev;

void init_sensors() {
    if (dht_init(&dev, &DHT_PIN, DHT11) != 0) {
        puts("Error initializing DHT11 sensor");
        return;
    }
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void read_sensor_data() {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != 0) {
        puts("Error reading data from DHT11 sensor");
        return;
    }
    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

    if (temp > TEMP_THRESHOLD) {
        puts("Temperature above threshold, turning on LED.");
        gpio_set(LED_PIN);
    } else {
        puts("Temperature below threshold, turning off LED.");
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    init_sensors();
    while (1) {
        read_sensor_data();
        ztimer_sleep(ZTIMER_SEC, DELAY);
    }
    return 0;
}