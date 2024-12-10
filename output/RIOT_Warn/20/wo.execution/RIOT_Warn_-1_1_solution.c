#include <stdio.h>  
#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO pin for the LED
#define TEMP_THRESHOLD 30       // Threshold temperature in Celsius
#define SLEEP_DURATION (5U * US_PER_SEC)  // 5 seconds

dht_t dev;

void read_and_warn(void){
    int16_t temp, hum;
    
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Error reading DHT sensor\n");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: Temperature exceeds threshold! LED turned ON\n");
    } else {
        gpio_clear(LED_PIN);
        printf("Temperature is below threshold. LED turned OFF\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    dht_params_t params;
    params.pin = GPIO_PIN(0, 3);  // Assume pin for DHT
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }
    
    while (1) {
        read_and_warn();
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}