
#include "thread.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "xtimer.h"

#define DHT11_PIN            GPIO_PIN(0, 18)
#define LED_PIN              GPIO_PIN(0, 21)
#define TEMP_THRESHOLD       25
#define SLEEP_INTERVAL       5

static void read_dht11(void *arg) {
    (void)arg;
    while (1) {
        int temperature = 0;
        // Simulate reading temperature from DHT11
        // Replace this with actual DHT11 reading logic
        temperature = 26; // Example temperature value

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to read DHT11 sensor
    thread_create(
        thread_stack(NULL, 512),
        512,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        read_dht11,
        NULL,
        "dht11_reader"
    );

    return 0;
}
