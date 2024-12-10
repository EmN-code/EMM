#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN     GPIO_PIN(PORT_A, 0)  // Adjust as per actual pin
#define LED_PIN     GPIO_PIN(PORT_B, 1)  // Adjust as per actual pin
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius
#define SLEEP_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

dht_t sensor;

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    int rc;

    while(1) {
        rc = dht_read(&sensor, &temp, &hum);
        if (rc != DHT_OK) {
            printf("DHT read failed\n");
        }
        else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold, LED ON\n");
            } 
            else {
                gpio_clear(LED_PIN);
                printf("Temperature below threshold, LED OFF\n");
            }
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    int rc;

    printf("Initializing DHT sensor on pin %d\n", DHT_PIN);
    rc = dht_init(&sensor, DHT_PIN, DHT11);

    if (rc != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    read_dht_and_control_led();

    return 0;
}