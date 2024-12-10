#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN 4 // Example pin for DHT
#define LED_PIN 2 // Example pin for LED

void *dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    while (1) {
        int res = dht_read(&dev, &temperature, &humidity);
        if (res == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return NULL;
}

void initialize_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);
}

int main(void) {
    initialize_gpio();
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_read_thread, NULL, "dht_reader");
    return 0;
}