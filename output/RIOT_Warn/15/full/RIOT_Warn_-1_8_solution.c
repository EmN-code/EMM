#include "string.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 30
#define SLEEP_DURATION (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

int main(void) {
    (void)puts("Starting temperature monitoring with DHT11 sensor...");

    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    int init_res;
    while ((init_res = dht_init(&dev, &params)) != DHT_OK) {
        printf("DHT init failed with error %d, retrying...\n", init_res);
        xtimer_sleep(1);
    }
    printf("DHT initialized successfully\n");
    
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                puts("Temperature exceeds threshold! Turning on LED.");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT sensor: %d\n", res);
        }
        xtimer_usleep(SLEEP_DURATION);
    }
    return 0;
}