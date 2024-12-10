#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN    GPIO_PIN(PORT_A, 0) // Assuming PORT_A and correct pin
#define LED_PIN    GPIO_PIN(PORT_B, 0) // Assuming PORT_B and correct pin
#define THRESHOLD  25 // Temperature threshold in Celsius

static dht_t dev;

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Error reading DHT sensor\n");
        return;
    }
    printf("Temperature: %d.%d°C\n", temp/10, temp%10);
    printf("Humidity: %d.%d%%\n", hum/10, hum%10);

    // Control LED based on temperature threshold
    if (temp/10 > THRESHOLD) {
        gpio_set(LED_PIN);
        printf("LED ON: Temperature exceeded threshold\n");
    } else {
        gpio_clear(LED_PIN);
        printf("LED OFF: Temperature below threshold\n");
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        read_dht_and_control_led();
        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}