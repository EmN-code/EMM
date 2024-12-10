#include <ztimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 9) // Adjust the pin according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 5) // Adjust the LED pin according to your setup
#define TEMP_THRESHOLD 30          // Temperature threshold for the warning

void init_sensors(void) {
    if (dht_init(&dht_dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT11\n");
    }
}

void read_dht11_and_control_led(void) {
    int16_t temperature;
    int16_t humidity;
    
    if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensors();

    while (1) {
        read_dht11_and_control_led();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}