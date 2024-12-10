#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assuming PORT_A is declared and GPIO 0 is used
#define LED_PIN GPIO_PIN(PORT_B, 0) // Assuming PORT_B is declared and GPIO 0 is used
#define TEMP_THRESHOLD 30 // Temperature threshold

static dht_t dht_device;
static int16_t temperature;
static int16_t humidity;

void init_peripherals(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dht_device, DHT_PIN, DHT11);
}

void read_dht_and_check_temperature(void) {
    if (dht_read(&dht_device, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        }
        else {
            gpio_clear(LED_PIN);
        }
    }
    else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    init_peripherals();

    while (1) {
        read_dht_and_check_temperature();
        xtimer_sleep(5);
    }

    return 0;
}