#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 10)
#define LED_PIN         GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD  30

static dht_t dht;

void init_sensors(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    dht_init(&dht, &params);
}

void manage_temperature(void) {
    int16_t temperature, humidity;
    while(1) {
        if (dht_read(&dht, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature/10, temperature%10, humidity/10, humidity%10);
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature threshold exceeded! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED is OFF.\n");
            }
        } else {
            puts("Failed to read sensor data\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensors();
    puts("Starting temperature monitoring\n");
    manage_temperature();
    return 0;
}