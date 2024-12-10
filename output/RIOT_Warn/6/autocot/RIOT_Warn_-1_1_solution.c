#include "xtensa/config/core-isa.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define LED pin
#define TEMP_THRESHOLD 30          // Temperature threshold in degrees Celsius
#define INTERVAL (5U * US_PER_SEC)

static dht_t dev;

void *read_dht(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check temperature and control LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }

        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    dht_params_t params = {
        .pin = GPIO_PIN(PORT_A, 0),  // Define the DHT sensor pin
        .type = DHT11    // Define DHT type
    };

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for reading DHT
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "read_dht");

    return 0;
}