#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 17) // DHT sensor pin
#define LED_PIN GPIO_PIN(0, 16) // LED pin
#define TEMP_THRESHOLD 30       // Temperature threshold
#define INTERVAL (5 * US_PER_SEC) // 5 seconds interval

static dht_t dht_dev;

void *dht_read_loop(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11 sensor\n");
        } else {
            // Convert temp from 0.1 degrees to full degrees
            temp /= 10;

            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            // Check temperature threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the next reading cycle
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dht_dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    printf("DHT11 initialized\n");

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to poll the DHT sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_read_loop, NULL, "dht_read_loop");

    return 0;
}