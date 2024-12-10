#include <stdio.h> // for printf
#include <errno.h> // error number definitions
#include <dht11.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT_PIN     GPIO_PIN(PORT_A, 0) // Define the correct port and pin for your setup
#define LED_PIN     GPIO_PIN(PORT_A, 1) // Define the correct port and pin for your setup
#define THRESHOLD   25                 // Temperature threshold to turn on LED

void read_dht_and_toggle_led(void) {
    int16_t temp, hum;
    dht_t dev;

    // Initialize DHT11 device
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return;
    }

    // Read temperature and humidity from DHT11
    if (dht_read(&dev, &temp, &hum) != 0) {
        printf("Failed to read data from DHT11\n");
        return;
    }
    
    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

    // Check the temperature and turn on/off the LED accordingly
    if (temp > THRESHOLD) {
        gpio_set(LED_PIN);
        printf("LED ON: Temperature exceeded threshold!\n");
    } else {
        gpio_clear(LED_PIN);
        printf("LED OFF: Temperature is below threshold.\n");
    }
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    printf("DHT11 and LED control initialized\n");

    while (1) {
        read_dht_and_toggle_led();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}