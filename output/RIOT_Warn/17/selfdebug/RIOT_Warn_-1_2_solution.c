#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_A, 4)
#define TEMP_THRESHOLD 30

void read_dht11(void)
{
    // Simulated read from DHT11 sensor, returns temperature in Celsius
    int temperature = 25; // Replace with actual DHT11 reading logic
    
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }

    printf("Temperature: %d°C\n", temperature);
}

int main(void)
{
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    // Run reading loop every 5 seconds
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}