
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      30
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds

static void read_dht11(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        while (1) {
            if (dht_read(&dev, &temp, &hum) == DHT_OK) {
                printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                       (int)temp / 10, (int)temp % 10, (int)hum / 10, (int)hum % 10);

                if (temp / 10 > TEMP_THRESHOLD) {
                    gpio_set(LED_PIN);
                } else {
                    gpio_clear(LED_PIN);
                }
            } else {
                puts("Failed to read from DHT11 sensor");
            }

            xtimer_usleep(SLEEP_INTERVAL);
        }
    } else {
        puts("Failed to initialize DHT11 sensor");
    }
}

int main(void) {
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to read DHT11 sensor
    thread_create(
        thread_stack_alloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        read_dht11,
        NULL,
        "dht11_reader"
    );

    return 0;
}
