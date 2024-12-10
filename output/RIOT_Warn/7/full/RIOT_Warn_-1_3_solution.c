#include <thread.h>
#include <event/thread.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 4) // Example GPIO pin for DHT11
#define LED_PIN GPIO_PIN(0, 5) // Example GPIO pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static char _thread_stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_PIN) != 0) {
        puts("DHT11 initialization failed");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            puts("Failed to read from DHT11");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    thread_create(_thread_stack, sizeof(_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  dht_thread, NULL, "dht_thread");

    return 0;
}