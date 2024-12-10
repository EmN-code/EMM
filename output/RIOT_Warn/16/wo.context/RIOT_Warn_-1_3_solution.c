#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 4)  // Replace with correct GPIO pin
#define LED_PIN         GPIO_PIN(0, 5)  // Replace with correct GPIO pin
#define TEMP_THRESHOLD  30              // Temperature threshold for LED warning
#define SLEEP_INTERVAL  (5U * 1000000U) // 5 seconds sleep interval in microseconds

dht_t dev;

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    dht_init(&dev, DHT_PIN, DHT11);

    while (1) {
        read_dht_and_control_led();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}