#include "shell.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_A, 1)   // GPIO pin for LED
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

static dht_t dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Failed to read from DHT11\n");
        return;
    }
    temp /= 10;
    hum /= 10;
    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: Temperature exceeds threshold! LED turned ON\n");
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    puts("RIOT DHT11 and LED control example\n");

    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}