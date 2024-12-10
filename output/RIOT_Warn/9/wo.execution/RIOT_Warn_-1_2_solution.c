#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "thread.h"
#include "dht.h"

#define DHT_PIN   GPIO_PIN(0, 4)  // Adjust according to your hardware setup
#define LED_PIN   GPIO_PIN(0, 5)  // Adjust according to your hardware setup
#define TEMP_THRESHOLD 30        // Temperature threshold in Celsius

static dht_t dev;

void *dht_reader_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the DHT sensor
    dht_init(&dev, DHT_PIN, DHT11);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the DHT reader thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_reader_thread, NULL, "dht_reader");

    return 0;
}