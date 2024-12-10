#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_B, 1) // Define the GPIO pin for the DHT sensor
#define LED_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 25 // Define the temperature threshold in Celsius

void check_temperature(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("DHT initialization failed\n");
        return;
    }

    // Read the sensor values
    if (dht_read(&dev, &temperature, &humidity) != 0) {
        printf("DHT read failed\n");
        return;
    }

    // Convert temperature to Celsius
    temperature /= 10;

    // Check temperature and turn on LED if it exceeds the threshold
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: High temperature! (%d°C)\n", temperature);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        check_temperature();
        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}