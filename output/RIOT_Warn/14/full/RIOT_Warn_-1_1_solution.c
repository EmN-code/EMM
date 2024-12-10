#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <gpio.h>

#define DHT_PIN     GPIO_PIN(0, 4)  // Specify the correct pin for DHT
#define LED_PIN     GPIO_PIN(0, 5)  // Specify the correct pin for the LED
#define TEMP_THRESHOLD 25           // Temperature threshold

void init_sensors(void) {
    if (dht_init(&dev, &params) != DHT11_OK) {
        printf("DHT sensor initialization failed\n");
    }
}

void check_temperature(void) {
    int16_t temp, hum;
    int result;

    while (1) {
        result = dht_read(&dev, &temp, &hum);
        if (result == DHT11_OK) {
            printf("Temp: %d Hum: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        } else {
            printf("Failed to read the sensor\n");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensors();
    check_temperature();
    return 0;
}