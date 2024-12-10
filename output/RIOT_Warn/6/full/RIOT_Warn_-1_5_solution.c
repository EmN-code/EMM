#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "thread.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_A, 5)

void *read_dht(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Sleep for approximately 5 seconds using a busy-wait loop
        for (int i = 0; i < 50; ++i) {
            xtimer_usleep(100000); // 0.1 second
        }
    }
    return NULL;
}

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("DHT sensor initialization failed\n");
        return 1;
    }

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the DHT reading thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  read_dht, NULL, "dht_reader");

    return 0;
}