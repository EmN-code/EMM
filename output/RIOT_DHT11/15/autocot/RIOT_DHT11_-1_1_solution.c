#include "timex.h"
#include "thread.h"
#include "dht.h"
#include "esp_idf_gpio_config.h"

#define GPIO_PIN 2
#define DHT_TYPE DHT11
#define MEASUREMENT_INTERVAL (5U * US_PER_SEC)  // 5 seconds

// DHT device descriptor
static dht_t dev;

// Buffer for the DHT thread stack
static char stack[THREAD_STACKSIZE_MAIN];

// Thread function to read DHT values
void *read_dht(void *arg) {
    (void)arg;  // Unused
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        xtimer_usleep(MEASUREMENT_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO pin
    const gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << GPIO_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    esp_idf_gpio_config(&cfg);

    // Initialize DHT sensor
    dht_params_t params;
    dht_params_init(&params, DHT_TYPE);
    params.pin = GPIO_PIN;
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Create a thread to periodically read from the sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht, NULL, "read_dht");

    // The main thread can perform other tasks here

    return 0;
}