#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4) // Assume the DHT11 is connected to pin GPIO4
#define LED_PIN GPIO_PIN(0, 5) // Assume the LED is connected to pin GPIO5
#define TEMP_THRESHOLD 30

static void *temperature_monitor(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(DHT11, DHT_PIN, &temp, &hum) == 0) {
            temp /= 10; // Convert to integer form
            if (temp >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                // Ensure LED is turned off otherwise
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5000 milliseconds
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, temperature_monitor, NULL, "temp_monitor");
    return 0;
}