#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(PORT_A, 2)  // Placeholder pin, please set accordingly
#define LED_PIN GPIO_PIN(PORT_B, 5)  // Placeholder pin, please set accordingly
#define TEMP_THRESHOLD 30  // Temperature threshold for warning

// Device descriptor
static dht_t dev;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    printf("Initializing DHT sensor\n");

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    while (1) {
        pause();  // Keep main thread alive
    }

    return 0;
}