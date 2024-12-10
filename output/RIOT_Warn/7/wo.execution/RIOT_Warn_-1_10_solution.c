#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "dht.h"
#include "dht_params.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "timex.h"

#define DHT11_PIN   GPIO_PIN(0, 4)
#define LED_PIN     GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

void *dht11_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT11 sensor
    dht_params_t params;
    params.pin = DHT11_PIN;
    dht_init(&dev, &params);
    
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check temperature threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create thread for reading the DHT11 sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht11_thread, NULL, "dht11");

    return 0;
}