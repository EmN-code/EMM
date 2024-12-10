#include <thread_flags.h>
#include <thread_config.h>
#include <esp_idf_rtc_gpio_pullup_dis.h>
#include <esp_idf_gpio_intr_disable.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN 2
#define SLEEP_INTERVAL (5 * XTIMER_SEC)

void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    printf("Starting DHT11 reader\n");
    read_dht11();

    return 0;
}