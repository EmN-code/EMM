/* This code is for ESP32 using RIOT OS, which reads data from a DHT11 sensor every 5 seconds and turns on an LED if the temperature exceeds a threshold. */

#include "xtimer.h"
#include "timex.h"
#include "periph/dht.h"
#include "periph/gpio.h"

#define TEMP_THRESHOLD 30
#define READ_INTERVAL 5
#define DHT_PIN GPIO_PIN(PORT_A, 11)
#define LED_PIN GPIO_PIN(PORT_B, 22)

static dht_t dev;

/* Initialize the DHT sensor and GPIO for LED */
int init_devices(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    if (dht_init(&dev, &params) != DHT_OK) {
        return -1;
    }
    gpio_init(LED_PIN, GPIO_OUT);
    return 0;
}

/* Function to read temperature and control LED */
void read_temp_and_control_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d | Humidity: %d\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading sensor\n");
    }
}

int main(void) {
    if (init_devices() != 0) {
        printf("Error initializing devices\n");
        return 1;
    }

    while (1) {
        read_temp_and_control_led();
        xtimer_sleep(READ_INTERVAL);
    }
    return 0;
}