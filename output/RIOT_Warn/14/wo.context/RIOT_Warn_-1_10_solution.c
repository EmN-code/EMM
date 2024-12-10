#include <xtimer.h>
#include <periph/gpio.h>

#define DHT_PIN     GPIO_PIN(PORT_A, 2)    // Correct GPIO definition for ESP32
#define LED_PIN     GPIO_PIN(PORT_B, 3)    // Correct GPIO definition for ESP32
#define THRESHOLD   30   // Temperature threshold

static dht_t dev;

void read_sensor_and_control_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        temp /= 10; // Convert to proper format if necessary
        if (temp > THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("DHT error\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        read_sensor_and_control_led();
        xtimer_sleep(5);
    }

    return 0;
}