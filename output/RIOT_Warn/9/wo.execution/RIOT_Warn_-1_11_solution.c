#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 5)  // Adjust this to your actual pin
#define LED_PIN GPIO_PIN(0, 10) // Adjust this to your actual LED pin
#define TEMP_THRESHOLD 30       // Temperature threshold in degree Celsius

static char stack[THREAD_STACKSIZE_DEFAULT];

void *dht_read_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    dht_init(&dev, DHT11, DHT_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to read the DHT sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_read_thread, NULL, "dht_reader");

    // Main thread does nothing, everything is handled by the DHT thread
    while (1) {
        thread_yield();
    }
    return 0;
}