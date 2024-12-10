#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"


#define DHT11_PIN 22        // Assume GPIO22 is connected to DHT11
#define LED_PIN 23          // Assume GPIO23 is connected to an LED
#define TEMP_THRESHOLD 30   // Temperature threshold in degrees Celsius

static char _thread_stack[THREAD_STACKSIZE_MAIN];

void *read_dht11_thread(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    dev.pin = DHT11_PIN;

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) == 0) {
            // Temperature reading is in tenths of degrees Celsius
            if (temp/10 > TEMP_THRESHOLD) {
                // If the temperature exceeds the threshold, turn on the LED
                gpio_set(LED_PIN);
            }
            else {
                // Otherwise, turn off the LED
                gpio_clear(LED_PIN);
            }
        }
        // Sleep for 5 seconds
        xtimer_usleep(5000000);
    }

    return NULL;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11
    dht_t dev;
    dev.params = DHT_PARAMS;

    // Create a thread for reading the DHT11 sensor
    thread_create(_thread_stack, sizeof(_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11_thread, NULL, "dht11_reader");

    // Main thread does nothing, just let the created thread run
    while (1) {
        xtimer_usleep(1000000);
    }

    return 0;
}
