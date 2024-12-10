#include "ztimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "msg.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Example pin, you should change it according to your setup
#define LED_PIN GPIO_PIN(0, 5)  // Example pin, you should change it according to your setup
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

void *dht_thread(void *arg) {
    (void)arg;  // Silence compiler warning about unused variable

    dht_t dev;
    dev.pin = DHT_PIN;
    int16_t temperature, humidity;

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_SECOND, 5);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}