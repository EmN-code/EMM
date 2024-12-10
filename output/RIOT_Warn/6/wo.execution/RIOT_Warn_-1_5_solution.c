#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"
#include "periph/gpio.h"

#define TEMP_THRESHOLD 300  // threshold temperature in dC (30.0 °C)
#define SLEEP_INTERVAL (5 * US_PER_SEC)  // 5 seconds sleep
#define LED_PIN GPIO_PIN(1)
#define DHT_PIN GPIO_PIN(2)
#define DHT_TYPE DHT11

void *dht_thread(void *arg) {
    (void) arg;

    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, &dht_params[DHT_TYPE][DHT_PIN]) != DHT_OK) {
        // Error during initialization
        return NULL;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    // Create a simple thread to read DHT sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}