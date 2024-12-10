#include "ztimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Assuming DHT11 is connected to PORT_A, PIN 0
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Assuming LED is connected to PORT_B, PIN 0
#define TEMP_THRESHOLD 300           // 30.0°C expressed in deci-degrees Celsius

static dht_t dht_dev;

void *dht_reader_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize GPIOs
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize DHT device
    dht_init(&dht_dev, DHT_PIN);

    // Create thread for reading DHT sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_reader_thread, NULL,
                  "dht_reader");

    // Placeholder for other application code
    while(1) {
        // Do other tasks here, if any
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep to yield CPU
    }

    return 0;
}