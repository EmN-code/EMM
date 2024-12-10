#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <dht.h>

#define DHT11_PIN         GPIO_PIN(PORT_A, 7)
#define TEMP_THRESHOLD    30  // Temperature threshold in degrees Celsius
#define LED_PIN           GPIO_PIN(PORT_A, 8)

static dht_t dev;

void init_sensors(void) {
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);
    gpio_init(LED_PIN, GPIO_OUT);
}

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature exceeded threshold, turning on LED\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature below threshold, LED is off\n");
        }
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    init_sensors();
    while (1) {
        read_dht11();
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    return 0;
}