#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_NUM_4 // Example GPIO pin for DHT11
#define LED_PIN GPIO_NUM_2 // Example GPIO pin for LED
#define TEMP_THRESHOLD 25 // Temperature threshold

void *dht_thread(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&temp, &hum) == 0) { // Assuming dht_read takes addresses of int16_t
            if (temp / 10.0 > TEMP_THRESHOLD) { // Temperature value might need conversion from read value
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }
        ztimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO for DHT and LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for reading DHT sensor data
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    return 0;
}