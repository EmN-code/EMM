#include "xtimer.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Specify an actual GPIO pin number for your setup.
#define LED_PIN GPIO_PIN(PORT_B, 0)   // Specify an actual GPIO pin number for your setup.
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

void read_dht11(void)
{
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Error reading from DHT11\n");
        return;
    }

    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void)
{
    dht_t dev;
    dev.params.pin = DHT11_PIN; // Correct member usage for dht_t struct
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}