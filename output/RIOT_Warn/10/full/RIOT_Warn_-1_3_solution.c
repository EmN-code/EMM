#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "dht.h"

#define DHT11_PIN 4
#define LED_PIN 5
#define TEMP_THRESHOLD 30

void setup_dht(void) {
    dht_t dev;
    int init_status = dht_init(&dev, 0, DHT11_PIN, DHT_TYPE_DHT11);
    while (init_status != 0) {
        printf("Failed to initialize DHT11. Retrying...\n");
        init_status = dht_init(&dev, 0, DHT11_PIN, DHT_TYPE_DHT11);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("DHT11 initialized successfully.\n");
}

void read_temperature_and_control_led(void) {
    dht_t dev;
    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_init(&dev, 0, DHT11_PIN, DHT_TYPE_DHT11);

    while (1) {
        int read_status = dht_read(&dev, &temperature, &humidity);
        if (read_status == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to get temperature from DHT11. Retrying...\n");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void) {
    setup_dht();
    gpio_init(LED_PIN, GPIO_OUT);
    read_temperature_and_control_led();
    return 0;
}