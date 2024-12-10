#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht/dht.h>

#define DHT_PIN GPIO_PIN(PORT_B, 2) // Adjust as needed
#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TEMPERATURE_THRESHOLD 30

dht_t dev;

void read_temperature(void)
{
    int16_t temperature;
    int16_t humidity;

    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temperature / 10, temperature % 10, 
                humidity / 10, humidity % 10);
        if (temperature / 10 > TEMPERATURE_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading data from DHT11\n");
    }
}

int main(void)
{
    printf("Starting DHT11 sensor reading task\n");
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dev, DHT_PIN, DHT11);
    
    while (1) {
        read_temperature();
        xtimer_sleep(5);
    }
    
    return 0;
}