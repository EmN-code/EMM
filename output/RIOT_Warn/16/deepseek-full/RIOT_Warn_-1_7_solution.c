
#include "thread.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "event/thread.h"

#define LED_PIN            GPIO_PIN(PORT_B, 0)
#define DHT_PIN            GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD     30
#define SLEEP_DURATION     5000000

static void *dht_thread(void *arg)
{
    (void)arg;
    int16_t temperature, humidity;
    gpio_t led = LED_PIN;
    gpio_t dht = DHT_PIN;

    gpio_init(led, GPIO_OUT);
    gpio_init(dht, GPIO_IN);

    while (1) {
        // Simulate reading DHT11 data
        temperature = 25; // Replace with actual reading logic
        humidity = 50;    // Replace with actual reading logic

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(led);
        } else {
            gpio_clear(led);
        }

        // Sleep for 5 seconds
        thread_sleep(SLEEP_DURATION);
    }

    return NULL;
}

int main(void)
{
    thread_create(thread_stack(dht_thread), sizeof(thread_stack(dht_thread)),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  dht_thread, NULL, "dht_thread");

    return 0;
}
