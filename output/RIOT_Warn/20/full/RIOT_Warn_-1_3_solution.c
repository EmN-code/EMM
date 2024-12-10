#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 5)  // Example pin for DHT11
#define LED_PIN GPIO_PIN(0, 2)  // Example pin for LED
#define TEMP_THRESHOLD 30       // Temperature threshold in degrees Celsius

static dht_t dev;

gpio_t dht_gpio_pin = DHT_PIN;
gpio_t led_gpio_pin = LED_PIN;

void init_sensors(void) {
    while (1) {
        if (dht_init(&dev, dht_gpio_pin) == DHT_OK) {
            printf("DHT11 initialized successfully.");
            break;
        } else {
            printf("Failed to initialize DHT11, retrying in 1 second.\n");
            xtimer_sleep(1);
        }
    }
    gpio_init(led_gpio_pin, GPIO_OUT);
}

void read_sensor_and_control_led(void) {
    int16_t temperature;
    int16_t humidity;

    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        if (temperature >= TEMP_THRESHOLD) {
            gpio_set(led_gpio_pin);
        } else {
            gpio_clear(led_gpio_pin);
        }
    } else {
        printf("Failed to read from DHT11 sensor.\n");
    }
}

int main(void) {
    init_sensors();
    while (1) {
        read_sensor_and_control_led();
        xtimer_sleep(5);
    }
    return 0;
}