#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "dht_params.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define DHT_PIN 4        // GPIO pin for DHT data
#define LED_PIN 2        // GPIO pin for LED
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

void *dht_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity from DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            
            // Check if the temperature exceeds the threshold
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        ztimer_sleep(ZTIMER_SEC, 5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    return 0;
}